#include "Game.hpp"

#include <format>
#include <optional>
#include <string>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_messagebox.h>
#include <SDL_mouse.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#ifdef _WIN32
#ifdef WIN32_LEAN_AND_MEAN
#include "windows.h"
#else
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#undef WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#endif // _WIN32

#include "ResourceManager.hpp"
#include "Shader.hpp"

#ifdef _WIN32
std::optional<std::wstring> get_executable_path() {
    wchar_t temp_outpath[MAX_PATH];
    if(GetModuleFileNameW(NULL, temp_outpath, MAX_PATH) == MAX_PATH) {
        return std::nullopt;
    }
    return std::wstring(temp_outpath);
}
#elif __linux__
std::optional<std::wstring> get_executable_path() {
    return std::filesystem::read_symlink("/proc/self/exe").generic_wstring();
}
#endif // _WIN32

Game::Game() :
    k_window_width{800.0}, k_window_height{600.0}, exiting_{false}, context_{nullptr}, entity_manager_{},
    component_manager_{}, input_manager_{}, systems_{}, delta_time_{0},
    window_{NULL}, prev_time_{SDL_GetPerformanceCounter()}, shader_program_{0}
{
    auto executable_path = get_executable_path();
    if(!executable_path) {
        fatal_error("Executable path too long.");
    }
    runtime_dir_ = get_executable_path().value();
    runtime_dir_.remove_filename();

    resource_manager_.asset_directory = runtime_dir_ / "../assets";

    Logger::set_base_path(runtime_dir_);
    //Logger::set_default(Logger::create_logger("engine.log", Logger::LogLevel::debug));
    Logger::set_default(Logger::create_console_logger(Logger::LogLevel::debug));
    Logger* logger = Logger::get_default();
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fatal_error(std::string("Could not init SDL: ").append(SDL_GetError()));
    }

    window_ = SDL_CreateWindow(
        "not-doom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, k_window_width, k_window_height, SDL_WINDOW_OPENGL);
    if (window_ == NULL) {
        fatal_error(std::string("Could not create window: ").append(SDL_GetError()));
    }

    context_ = SDL_GL_CreateContext(window_);
    if (context_ == NULL) {
        fatal_error(std::string("Could not create OpenGL context: ").append(SDL_GetError()));
    }

    switch(GLenum result = glewInit()){
        case GLEW_OK: {
            logger->info("GLEW initialized.");
            break;
        }
        case GLEW_ERROR_NO_GL_VERSION: {
            fatal_error("GLEW: missing GL version.");
            break;
        }
        case GLEW_ERROR_GL_VERSION_10_ONLY: {
            fatal_error("GLEW: Need at least OpenGL 1.1.");
            break;
        }
        case GLEW_ERROR_GLX_VERSION_11_ONLY: {
            fatal_error("GLEW: Need at least GLX 1.2.");
            break;
        }
        case GLEW_ERROR_NO_GLX_DISPLAY: {
            fatal_error("GLEW: Need GLX display for GLX support.");
            break;
        }
        default: {
            fatal_error("Unknown GLEW error.");
            break;
        }
    }

    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
    if (SDL_SetRelativeMouseMode(SDL_TRUE)) {
        logger->warning(std::string("Could not set relative mouse mode: ").append(SDL_GetError()));
    }

    resource_manager_.set_context(context_);
}

Game::~Game()
{
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

auto Game::fatal_error(const std::string& error_msg) -> void
{
    const Logger* logger = Logger::get_default();
    if (logger != nullptr) [[likely]] {
        logger->error(error_msg);
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error_msg.c_str(), window_);
    exiting_ = true;
}

auto Game::start() -> void
{
    setup();
    while (!exiting_) {
        loop();
        SDL_GL_SwapWindow(window_);
    }
}

auto Game::get_current_time() const -> float
{
    return static_cast<float>(current_time_) / static_cast<float>(SDL_GetPerformanceFrequency());
}

auto Game::loop() -> void
{
    current_time_ = SDL_GetPerformanceCounter();
    delta_time_ = static_cast<float>(current_time_ - prev_time_) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency());
    prev_time_ = current_time_;

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
