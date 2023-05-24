#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {
    private:
        static SDL_Renderer *renderer_;
        static bool exiting_;
        static void loop();
    public:
        static void start();
};

#endif // GAME_H
