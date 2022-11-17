#include "PauseState.h"

#include <GameEngine.h>
#include <States/GameState.h>
#include <States/MainMenuState.h>

void PauseState::init(GameEngine* gameEngine) {
    renderer = gameEngine->renderer;

    pausescreen = IMG_LoadTexture(renderer, "res/Menus/pausemenu.png");
    pointer = IMG_LoadTexture(renderer, "res/HUD/pointer.png");

    pointerRect.x = 58;
    pointerRect.y = 372;

    pointerRect.w = 32;
    pointerRect.h = 32;
}

void PauseState::destroy() {
    SDL_DestroyTexture(pausescreen);
    SDL_DestroyTexture(pointer);
}

void PauseState::pause() {}

void PauseState::resume() {}

void PauseState::handleInput(GameEngine* gameEngine) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.key.type == SDL_KEYUP) {
            switch(e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    gameEngine->popState();
                    break;

                case SDLK_w:
                    if(pointerPos > 0) pointerPos--;
                    break;

                case SDLK_s:
                    if(pointerPos < 2) pointerPos++;
                    break;

                case SDLK_SPACE:
                case SDLK_RETURN:
                    if(pointerPos == 0) {
                        gameEngine->popState();
                    } else if(pointerPos == 1) {
                        gameEngine->popState();
                        gameEngine->changeState(MainMenuState::getInstance());
                    } else if(pointerPos == 2) {
                        gameEngine->quit();
                    }
                    break;
            }
        }
    }
}

void PauseState::update(GameEngine* gameEngine) {
    if(pointerPos == 0) {
        pointerRect.y = 372;
    } else if(pointerPos == 1) {
        pointerRect.y = 497;
    } else if(pointerPos == 2) {
        pointerRect.y = 550;
    }
}

void PauseState::render(GameEngine* gameEngine) {
    SDL_RenderClear(gameEngine->renderer);

    SDL_RenderCopy(renderer, pausescreen, NULL, NULL);
    SDL_RenderCopy(renderer, pointer, NULL, &pointerRect);

    SDL_RenderPresent(gameEngine->renderer);
}
