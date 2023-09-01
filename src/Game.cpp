#include <PhysicsComponent.hpp>
#include <SDL.h>
#include <Game.hpp>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_video.h>
#include <SDL2/SDL_image.h>
#include <format>
#include <stdexcept>
#include <unordered_map>

const std::unordered_map<SDL_Keycode, Player::WalkingDirection> Game::kKeyToDirection =
{
    { SDLK_w, Player::WalkingDirection::kForward },
    { SDLK_a, Player::WalkingDirection::kLeft },
    { SDLK_s, Player::WalkingDirection::kBackward },
    { SDLK_d, Player::WalkingDirection::kRight },
};

Game::Game()
    : kWindowWidth(800.0)
    , kWindowHeight(600.0)
    , renderer_(nullptr)
    , player_( 0.0, 0.0, 0.0 )
    , camera_(player_.GetPhysicsRef(), kWindowWidth, kWindowHeight)
    , exiting_(false)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window_ = SDL_CreateWindow(
        "not-doom",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        kWindowWidth,
        kWindowHeight,
        0
    );

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, SDL_ALPHA_OPAQUE);
}

Game::~Game()
{
    for(auto texture : textures_){
        SDL_DestroyTexture(texture.second);
    }
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

auto Game::LoadTexture(std::string texture_name) -> SDL_Texture*
{
    SDL_Texture *temp;
    try {
        temp = textures_.at(texture_name);
    } catch (std::out_of_range const&) {
        temp = IMG_LoadTexture(renderer_, std::format("../assets/patches/{}.png", texture_name).c_str());
        textures_[texture_name] = temp;
    }
    return temp;
}

void Game::start(){
    while(!exiting_){
        loop();
    }
}

void Game::loop(){
    uint64_t curr_time = SDL_GetPerformanceCounter();
    uint64_t delta_time = (curr_time - prev_time_) * 1000 / SDL_GetPerformanceFrequency();
    (void)delta_time;
    prev_time_ = curr_time;

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT:
                switch(event.window.event){
                    case SDL_WINDOWEVENT_CLOSE:
                        exiting_ = true;
                        break;
                }
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                SDL_Keycode key = event.key.keysym.sym;
                if(event.key.repeat){
                    break;
                }
                switch(key){
//                    case SDLK_w:
//                    case SDLK_a:
//                    case SDLK_s:
//                    case SDLK_d:
//                        if(event.type == SDL_KEYDOWN){
//                            player_.SetWalkingDirection(Game::kKeyToDirection.at(key));
//                        } else {
//                            player_.RemoveWalkingDirection(Game::kKeyToDirection.at(key));
//                        }
//                        break;
                    case SDLK_SPACE:
                        if (event.type == SDL_KEYDOWN) {
                            player_.Jump();
                        }
                }
                break;
        }
    }

    const uint8_t* key_states = SDL_GetKeyboardState(nullptr);
    player_.SetWalkingDirection(Player::WalkingDirection::kForward, key_states[SDL_SCANCODE_W]);
    player_.SetWalkingDirection(Player::WalkingDirection::kLeft, key_states[SDL_SCANCODE_A]);
    player_.SetWalkingDirection(Player::WalkingDirection::kBackward, key_states[SDL_SCANCODE_S]);
    player_.SetWalkingDirection(Player::WalkingDirection::kRight, key_states[SDL_SCANCODE_D]);

    player_.Update();

    SDL_RenderClear(renderer_);

    SDL_Texture* brick1 = LoadTexture("brick");
    SDL_Texture* brick2 = LoadTexture("brick1");
    SDL_Texture* brick3 = LoadTexture("brick2");
    SDL_Texture* brick4 = LoadTexture("wood");
    Vec3 brick1_pos( -kWindowWidth / 2, kWindowHeight / 2, 0 );
    Vec3 brick2_pos( kWindowWidth / 2 - 50, kWindowHeight / 2, 0 );
    Vec3 brick3_pos( -kWindowWidth / 2, -kWindowHeight / 2 + 50, 0 );
    Vec3 brick4_pos( kWindowWidth / 2 - 50, -kWindowHeight / 2 + 50, 0 );
    Vec2 brick1_pos_view = camera_.TransformToView(brick1_pos);
    Vec2 brick2_pos_view = camera_.TransformToView(brick2_pos);
    Vec2 brick3_pos_view = camera_.TransformToView(brick3_pos);
    Vec2 brick4_pos_view = camera_.TransformToView(brick4_pos);
    SDL_Rect rect1 = { static_cast<int>(brick1_pos_view.x), static_cast<int>(brick1_pos_view.y), 50, 50 };
    SDL_Rect rect2 = { static_cast<int>(brick2_pos_view.x), static_cast<int>(brick2_pos_view.y), 50, 50 };
    SDL_Rect rect3 = { static_cast<int>(brick3_pos_view.x), static_cast<int>(brick3_pos_view.y), 50, 50 };
    SDL_Rect rect4 = { static_cast<int>(brick4_pos_view.x), static_cast<int>(brick4_pos_view.y), 50, 50 };
    SDL_RenderCopy(renderer_, brick1, NULL, &rect1);
    SDL_RenderCopy(renderer_, brick2, NULL, &rect2);
    SDL_RenderCopy(renderer_, brick3, NULL, &rect3);
    SDL_RenderCopy(renderer_, brick4, NULL, &rect4);

    SDL_RenderPresent(renderer_);
}
