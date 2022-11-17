#include "LevelCreatorSelector.h"

#include <GameEngine.h>
#include <States/GameState.h>
#include <States/LevelCreatorState.h>


void LevelCreatorSelector::init(GameEngine* gameEngine){
    renderer = gameEngine->renderer;

    SDL_SetWindowPosition(gameEngine->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    pointer = IMG_LoadTexture(renderer, "res/HUD/pointer.png");
    pointerLeftRect.x = 20;
    pointerLeftRect.y = 290;
    pointerLeftRect.w = 64;
    pointerLeftRect.h = 64;

    pointerRightRect.x = 716;
    pointerRightRect.y = 290;
    pointerRightRect.w = 64;
    pointerRightRect.h = 64;

    for(int i=0; i<6; i++){
        backgrounds[i] = IMG_LoadTexture(renderer, (backgroundParentPath + backgroundFiles[i]).c_str());
    }

    returnFont = TTF_OpenFont("res/Fonts/return.ttf", 48);
    whiteColor.r = 255;
    whiteColor.g = 255;
    whiteColor.b = 255;

    SDL_Surface* tmp = TTF_RenderText_Solid(returnFont, "Create", whiteColor);
    createText = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(createText, NULL, NULL, &createRect.w, &createRect.h);
    createRect.x = 600;
    createRect.y = 550;

    createSelectedRect.x = 560;
    createSelectedRect.y = 550;
    createSelectedRect.w = 32;
    createSelectedRect.h = 32;

    sizeMinusRect.x = 345;
    sizeMinusRect.y = 250;
    sizeMinusRect.w = 32;
    sizeMinusRect.h = 32;

    sizePlusRect.x = 450;
    sizePlusRect.y = 250;
    sizePlusRect.w = 32;
    sizePlusRect.h = 32;

    sizeRect.x = 390;
    sizeRect.y = 250;

}

void LevelCreatorSelector::destroy(){
    SDL_DestroyTexture(pointer);
    for(int i=0; i<6; i++) SDL_DestroyTexture(backgrounds[i]);
}

void LevelCreatorSelector::pause(){

}

void LevelCreatorSelector::resume(){

}

void LevelCreatorSelector::handleInput(GameEngine* gameEngine){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.quit.type == SDL_QUIT){
            gameEngine->quit();
        }

        if(e.key.keysym.sym == SDLK_ESCAPE){
            gameEngine->changeState(MainMenuState::getInstance());
        }

        if(e.motion.type == SDL_MOUSEMOTION){
            int x = e.motion.x;
            int y = e.motion.y;
            if(x > 600 && x < 600+createRect.w && y > 550 && y < 550+createRect.h){
                createSelected = true;
            }else{
                createSelected = false;
            }
        }

        if(e.button.type == SDL_MOUSEBUTTONDOWN){
            if(createSelected){
                //CREATE
                gameEngine->levelCreatorData.backgroundPath = backgroundParentPath+backgroundFiles[currentBackground];
                gameEngine->levelCreatorData.tilesetPath = "Assets/Tilesets/tileset0.png";
                gameEngine->levelCreatorData.levelWidth = levelSizeSelected;
                gameEngine->levelCreatorData.levelHeight = 20;
                gameEngine->levelCreatorData.tileSize = 32;
                //Switch state
                gameEngine->changeState(LevelCreatorState::getInstance());
            }


            if(e.motion.x > 20 && e.motion.x < 84){
                if(e.motion.y > 290 && e.motion.y < 354){
                    //clicked left pointer
                    if(currentBackground > 0) currentBackground--;
                }
            }

            if(e.motion.x > 716 && e.motion.x < 780){
                if(e.motion.y > 290 && e.motion.y < 354){
                    //clicked right pointer
                    if(currentBackground < 5) currentBackground++;
                }
            }

            if(e.motion.x > 345 && e.motion.x < 377 && e.motion.y > 250 && e.motion.y < 282){
                if(levelSizeSelected>25){
                    levelSizeSelected--;
                }
            }

            if(e.motion.x > 450 && e.motion.x < 482 && e.motion.y > 250 && e.motion.y < 282){
                if(levelSizeSelected<100){
                    levelSizeSelected++;
                }
            }

        }
    }
}

void LevelCreatorSelector::update(GameEngine* gameEngine){


}


void LevelCreatorSelector::render(GameEngine* gameEngine){

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgrounds[currentBackground], NULL, NULL);

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, pointer, NULL, &pointerLeftRect, 180.0, NULL, flip);
    SDL_RenderCopy(renderer, pointer, NULL, &pointerRightRect);


    SDL_RenderCopy(renderer, createText, NULL, &createRect);

    SDL_Texture* levelWidth;
    std::string str = "";
    int toStr = levelSizeSelected;
    do{
        str.insert(begin(str), (char)(toStr%10+48));
    }while(toStr/=10);
    SDL_Surface* tmp = TTF_RenderText_Solid(returnFont, str.c_str(), whiteColor);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

    SDL_QueryTexture(text, NULL, NULL, &sizeRect.w, &sizeRect.h);
    SDL_RenderCopy(renderer, text, NULL, &sizeRect);
    SDL_DestroyTexture(text);

    tmp = TTF_RenderText_Solid(returnFont, "Level Size", whiteColor);
    text = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    SDL_Rect lvlRect;
    lvlRect.x = 320;
    lvlRect.y = 210;
    SDL_QueryTexture(text, NULL, NULL, &lvlRect.w, &lvlRect.h);

    SDL_RenderCopy(renderer, text, NULL, &lvlRect);
    SDL_DestroyTexture(text);

    if(createSelected) SDL_RenderCopy(renderer, pointer, NULL, &createSelectedRect);

    SDL_RenderCopyEx(renderer, pointer, NULL, &sizeMinusRect, 180.0, NULL, flip);
    SDL_RenderCopy(renderer, pointer, NULL, &sizePlusRect);

    SDL_RenderPresent(renderer);


}
