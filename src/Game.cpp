#include "Game.hpp"

#include <cstddef>
#include <string>
#include <type_traits>

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include "ResourceManager.hpp"

#ifdef _WIN32
#ifdef WIN32_LEAN_AND_MEAN
#include "windows.h"
#else
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#undef WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#endif // _WIN32

#ifdef _WIN32
// std::wstring outpath
#define GET_EXECUTABLE_PATH(outpath) { \
    assert(typeof((outpath)) == std::wstring); \
    constexpr size_t _outpath_size = 256; \
    wchar_t _temp_outpath[_outpath_size]; \
    GetModuleFileNameA(NULL, _temp_outpath, _outpath_size); \
    (outpath).assign(_temp_outpath); \
}
#elif __linux__
// std::wstring outpath
#define GET_EXECUTABLE_PATH(outpath) { \
    static_assert(std::is_same<decltype(outpath), std::wstring>::value, "outpath must be std::wstring."); \
    (outpath).assign(std::filesystem::read_symlink("/proc/self/exe").generic_wstring()); \
}
#endif // _WIN32

Game::Game() :
    k_window_width{800.0}, k_window_height{600.0}, exiting_{false}, renderer_{nullptr}, entity_manager_{},
    component_manager_{}, input_manager_{}, systems_{},
    delta_time_{0}, logger_{LoggingManager::LogLevel::debug}, prev_time_{SDL_GetPerformanceCounter()}
{
    std::wstring executable_path;
    GET_EXECUTABLE_PATH(executable_path);
    runtime_dir_ = executable_path;
    runtime_dir_.remove_filename();

    resource_manager_.asset_directory = runtime_dir_ / "../assets";

    logger_.to_file(runtime_dir_ / "engine.log");
    resource_manager_.set_logger(&logger_);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logger_.error(std::string("Could not init SDL: ").append(SDL_GetError()));
    }

    window_ = SDL_CreateWindow(
        "not-doom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, k_window_width, k_window_height, 0);
    if (window_ == NULL) {
        logger_.error(std::string("Could not create window: ").append(SDL_GetError()));
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == NULL) {
        logger_.error(std::string("Could not create renderer: ").append(SDL_GetError()));
    }

    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        logger_.warning(std::string("Could not set relative mouse mode: ").append(SDL_GetError()));
    }

    resource_manager_.set_renderer(renderer_);
}

Game::~Game()
{
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::start()
{
    setup();
    while (!exiting_) {
        loop();
    }
}

void Game::loop()
{
    uint64_t curr_time = SDL_GetPerformanceCounter();
    delta_time_ = (curr_time - prev_time_) * 1000 / SDL_GetPerformanceFrequency();
    prev_time_ = curr_time;

    SDL_Event current_event;
    while (SDL_PollEvent(&current_event)) {
        switch (current_event.type) {
            case SDL_WINDOWEVENT: {
                handle_event_window(current_event.window);
                break;
            }
            case SDL_MOUSEMOTION: {
                handle_event_mouse_motion(current_event.motion);
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                handle_event_key(current_event.key);
                break;
            }
            default:
                break;
        }
    }

    for (const auto& system_function : systems_) {
        system_function();
    }
}
