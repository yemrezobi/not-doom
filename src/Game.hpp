#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <Player.hpp>
#include <unordered_map>
#include <string>

class Game {
    public:
        Game();
        ~Game();
        auto start() -> void;
        static const std::unordered_map<SDL_Keycode, Player::WalkingDirection> kKeyToDirection;
    private:
        auto LoadTexture(std::string texture_name) -> SDL_Texture*;
        const double kWindowWidth;
        const double kWindowHeight;
        SDL_Window *window_;
        SDL_Renderer *renderer_;
        Player player_;
        Camera camera_;
        std::unordered_map<std::string, SDL_Texture*> textures_;
        bool exiting_;
        uint64_t prev_time_;
        auto loop() -> void;
};

#endif // GAME_H
