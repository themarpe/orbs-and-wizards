#include "MainMenuState.h"

#include <GameEngine.h>
#include <States/GameState.h>
#include <States/LevelCreatorState.h>

#include "tinyfiledialogs.h"

void MainMenuState::init(GameEngine* gameEngine) {
    renderer = gameEngine->renderer;

    SDL_SetWindowPosition(gameEngine->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    mainscreen = IMG_LoadTexture(renderer, "res/Menus/mainmenu.png");
    pointer = IMG_LoadTexture(renderer, "res/HUD/pointer.png");

    pointerRect.x = 58;
    pointerRect.y = 325;

    pointerRect.w = 32;
    pointerRect.h = 32;
}

void MainMenuState::destroy() {
    SDL_DestroyTexture(mainscreen);
    SDL_DestroyTexture(pointer);
}

void MainMenuState::pause() {}

void MainMenuState::resume() {}

void MainMenuState::handleInput(GameEngine* gameEngine) {
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
                    if(pointerPos < 4) pointerPos++;
                    break;

                case SDLK_SPACE:
                case SDLK_RETURN:
                    if(pointerPos == 0) {
                        gameEngine->setLevelType(GameEngine::LevelType::NORMAL);

                        gameEngine->setLevelPath("Levels/level00.lvl");
                        gameEngine->setCurrentLevel(0);
                        gameEngine->changeState(GameState::getInstance());

                    } else if(pointerPos == 1) {
                        // WATCH REPLAY
                        char const* filterPatterns[2] = {"*.oawr", "*.*"};
                        char const* theOpenFilename =
                            tinyfd_openFileDialog("Select replay file to watch", "./Replays/*", 2, filterPatterns, "replay .oawr files", 0);

                        if(theOpenFilename) {
                            gameEngine->setReplayPath(theOpenFilename);
                            gameEngine->setLevelType(GameEngine::LevelType::REPLAY);
                            gameEngine->changeState(GameState::getInstance());
                        } else {
                            tinyfd_messageBox("Error", "No Replay file selected", "ok", "error", 1);
                        }

                    } else if(pointerPos == 2) {
                        // PlayLevel state
                        char const* filterPatterns[2] = {"*.lvl", "*.*"};
                        char const* theOpenFilename =
                            tinyfd_openFileDialog("Select the custom level", "./Custom Levels/*", 2, filterPatterns, "level .lvl files", 0);

                        if(theOpenFilename) {
                            gameEngine->setLevelType(GameEngine::LevelType::CUSTOM);
                            gameEngine->setLevelPath(theOpenFilename);
                            gameEngine->setCurrentLevel(0);
                            gameEngine->changeState(GameState::getInstance());
                        } else {
                            tinyfd_messageBox("Error", "No custom level selected", "ok", "error", 1);
                        }

                    } else if(pointerPos == 3) {
                        gameEngine->changeState(LevelCreatorSelector::getInstance());
                    } else if(pointerPos == 4) {
                        gameEngine->quit();
                    }
                    break;
            }
        }
    }
}

void MainMenuState::update(GameEngine* gameEngine) {
    if(pointerPos == 0) {
        pointerRect.y = 325;
    } else if(pointerPos == 1) {
        pointerRect.y = 380;
    } else if(pointerPos == 2) {
        pointerRect.y = 435;
    } else if(pointerPos == 3) {
        pointerRect.y = 490;
    } else if(pointerPos == 4) {
        pointerRect.y = 550;
    }
}

void MainMenuState::render(GameEngine* gameEngine) {
    SDL_RenderClear(gameEngine->renderer);

    SDL_RenderCopy(renderer, mainscreen, NULL, NULL);
    SDL_RenderCopy(renderer, pointer, NULL, &pointerRect);

    SDL_RenderPresent(gameEngine->renderer);
}
