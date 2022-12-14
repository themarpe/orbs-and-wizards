#include "GameOverState.h"

#include <GameEngine.h>
#include <States/GameState.h>
#include <States/LevelCreatorState.h>

void GameOverState::init(GameEngine* gameEngine) {
    renderer = gameEngine->renderer;

    SDL_SetWindowPosition(gameEngine->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    gamescreen = IMG_LoadTexture(renderer, "res/Menus/gameovermenu.png");
    pointer = IMG_LoadTexture(renderer, "res/HUD/pointer.png");

    pointerRect.x = 58;
    pointerRect.y = 325;

    pointerRect.w = 32;
    pointerRect.h = 32;
}

void GameOverState::destroy() {
    SDL_DestroyTexture(gamescreen);
    SDL_DestroyTexture(pointer);
}

void GameOverState::pause() {}

void GameOverState::resume() {}

void GameOverState::handleInput(GameEngine* gameEngine) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.quit.type == SDL_QUIT) {
            gameEngine->quit();
        }

        if(e.key.type == SDL_KEYUP) {
            switch(e.key.keysym.sym) {
                case SDLK_w:
                    if(pointerPos > 0) pointerPos--;
                    break;

                case SDLK_s:
                    if(pointerPos < 2) pointerPos++;
                    break;

                case SDLK_SPACE:
                case SDLK_RETURN:
                    if(pointerPos == 0) {
                        // save the replay
                        GameState::getInstance()->saveReplay();
                        gameEngine->changeState(MainMenuState::getInstance());
                    } else if(pointerPos == 1) {
                        gameEngine->changeState(MainMenuState::getInstance());
                    } else if(pointerPos == 2) {
                        gameEngine->quit();
                    }
                    break;
            }
        }
    }
}

void GameOverState::update(GameEngine* gameEngine) {
    if(pointerPos == 0) {
        pointerRect.y = 430;
    } else if(pointerPos == 1) {
        pointerRect.y = 490;
    } else if(pointerPos == 2) {
        pointerRect.y = 550;
    }
}

void GameOverState::render(GameEngine* gameEngine) {
    SDL_RenderClear(gameEngine->renderer);

    SDL_RenderCopy(renderer, gamescreen, NULL, NULL);
    SDL_RenderCopy(renderer, pointer, NULL, &pointerRect);

    SDL_RenderPresent(gameEngine->renderer);
}
