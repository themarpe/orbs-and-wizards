#include "MainMenuState.h"

#include <GameEngine.h>
#include <States/GameState.h>
#include <States/LevelCreatorState.h>


void MainMenuState::init(GameEngine* gameEngine){
    renderer = gameEngine->renderer;

    SDL_SetWindowPosition(gameEngine->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    mainscreen = IMG_LoadTexture(renderer, "res/Menus/mainmenu.png");
    pointer = IMG_LoadTexture(renderer, "res/HUD/pointer.png");

    pointerRect.x = 58;
    pointerRect.y = 325;

    pointerRect.w = 32;
    pointerRect.h = 32;

}

void MainMenuState::destroy(){
    SDL_DestroyTexture(mainscreen);
    SDL_DestroyTexture(pointer);
}

void MainMenuState::pause(){

}

void MainMenuState::resume(){

}

void MainMenuState::handleInput(GameEngine* gameEngine){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.quit.type == SDL_QUIT){
            gameEngine->quit();
        }

        if(e.key.type == SDL_KEYUP){
            switch(e.key.keysym.sym){
            case SDLK_w:
                if(pointerPos>0) pointerPos--;
                break;

            case SDLK_s:
                if(pointerPos<4) pointerPos++;
                break;

            case SDLK_SPACE:
            case SDLK_RETURN:
                if(pointerPos == 0){
                    gameEngine->setLevelType(GameEngine::LevelType::NORMAL);

                    gameEngine->setLevelPath("Levels/level00.lvl");
                    gameEngine->setCurrentLevel(0);
                    gameEngine->changeState(GameState::getInstance());


                }else if(pointerPos == 1){

                    //WATCH REPLAY
                    TCHAR NPath[MAX_PATH];
                    GetCurrentDirectory(MAX_PATH, NPath);
                    std::string customLevelDir(NPath);

                    std::cout << "\n current dir = " << customLevelDir << "\n";
                    char filename[ MAX_PATH ];

                      OPENFILENAME ofn;
                        ZeroMemory( &filename, sizeof( filename ) );
                        ZeroMemory( &ofn,      sizeof( ofn ) );
                        ofn.lStructSize  = sizeof( ofn );
                        ofn.hwndOwner    = NULL;  // If you have a window to center over, put its HANDLE here
                        ofn.lpstrFilter  = "Replay files\0*.oawr\0Any File\0*.*\0";
                        ofn.lpstrFile    = filename;
                        ofn.nMaxFile     = MAX_PATH;
                        ofn.lpstrTitle   = "Select the custom level";
                        ofn.Flags        = 0x02000000 | 0x00001000;

                      if (GetOpenFileName( &ofn ))
                      {

                        SetCurrentDirectoryA(NPath);
                        std::string replayPath(filename);
                        std::cout << "\n\nSELECTED = " << replayPath << "\n\n";
                        gameEngine->setReplayPath(replayPath);
                        gameEngine->setLevelType(GameEngine::LevelType::REPLAY);
                        gameEngine->changeState(GameState::getInstance());
                      }

                }
                else if(pointerPos == 2){

                    //PlayLevel state
                    TCHAR NPath[MAX_PATH];
                    GetCurrentDirectory(MAX_PATH, NPath);
                    std::string customLevelDir(NPath);
                    customLevelDir += "/CustomLevels";
                    std::cout << "\n custom dir = " << customLevelDir << "\n";
                    char filename[ MAX_PATH ];

                      OPENFILENAME ofn;
                        ZeroMemory( &filename, sizeof( filename ) );
                        ZeroMemory( &ofn,      sizeof( ofn ) );
                        ofn.lStructSize  = sizeof( ofn );
                        ofn.hwndOwner    = NULL;  // If you have a window to center over, put its HANDLE here
                        ofn.lpstrFilter  = "Level files\0*.lvl\0Any File\0*.*\0";
                        ofn.lpstrFile    = filename;
                        ofn.nMaxFile     = MAX_PATH;
                        ofn.lpstrTitle   = "Select the custom level";
                        ofn.Flags        = 0x02000000 | 0x00001000;
                        ofn.lpstrInitialDir = "C:/";

                      if (GetOpenFileName( &ofn ))
                      {
                        SetCurrentDirectory(NPath);
                        gameEngine->setLevelType(GameEngine::LevelType::CUSTOM);
                        std::string levelPath(filename);
                        std::cout << "\n\nSELECTED = " << levelPath << "\n\n";
                        gameEngine->setLevelPath(levelPath);
                        gameEngine->setCurrentLevel(0);
                        gameEngine->changeState(GameState::getInstance());
                      }


                }else if(pointerPos == 3){

                    gameEngine->changeState(LevelCreatorSelector::getInstance());
                }else if(pointerPos == 4){
                    gameEngine->quit();
                }
                break;
            }
        }
    }
}

void MainMenuState::update(GameEngine* gameEngine){

    if(pointerPos == 0){
        pointerRect.y = 325;
    }else if(pointerPos == 1){
        pointerRect.y = 380;
    }else if(pointerPos == 2){
        pointerRect.y = 435;
    }else if(pointerPos == 3){
        pointerRect.y = 490;
    }else if(pointerPos == 4){
        pointerRect.y = 550;
    }
}


void MainMenuState::render(GameEngine* gameEngine){
    SDL_RenderClear(gameEngine->renderer);

    SDL_RenderCopy(renderer, mainscreen, NULL, NULL);
    SDL_RenderCopy(renderer, pointer, NULL, &pointerRect);

    SDL_RenderPresent(gameEngine->renderer);
}
