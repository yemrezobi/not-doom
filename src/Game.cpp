#include <SDL.h>
#include <Game.hpp>
#include <SDL_events.h>
#include <SDL_video.h>

SDL_Renderer *Game::renderer_ = nullptr;
bool Game::exiting_ = false;

void Game::start(){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0);

    renderer_ = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, SDL_ALPHA_OPAQUE);

    while(!exiting_){
        loop();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::loop(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_WINDOWEVENT){
            switch(event.window.event){
                case SDL_WINDOWEVENT_CLOSE:
                    exiting_ = true;
            }
        }
    }

    SDL_RenderClear(renderer_);
    SDL_RenderPresent(renderer_);
}
