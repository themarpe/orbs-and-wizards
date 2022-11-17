#include <States/GameState.h>

#include <Level.h>

void GameState::init(GameEngine* gameEngine){

    time = SDL_GetTicks();

    if(gameEngine->getLevelType() == GameEngine::LevelType::NORMAL) game = new Game(gameEngine);
    else if(gameEngine->getLevelType() == GameEngine::LevelType::CUSTOM) game = new Game(gameEngine);
    else if(gameEngine->getLevelType() == GameEngine::LevelType::REPLAY) game = new Game(gameEngine, gameEngine->getReplayPath());


    TTF_Font* returnFont = TTF_OpenFont("res/Fonts/return.ttf", 42);
    SDL_Color whiteColor;
    whiteColor.r = 255;
    whiteColor.g = 255;
    whiteColor.b = 255;

    std::string tmpStr = "";
    int tmpInv = gameEngine->getCurrentLevel();
    do{
        tmpStr.insert(begin(tmpStr), (char)(tmpInv%10+48+1));
    }while(tmpInv /= 10);
    std::string newStr = "Level " + tmpStr;
    newStr+=":";

    SDL_Surface* tmp = TTF_RenderText_Solid(returnFont, newStr.c_str(), whiteColor);
    levelTTF = SDL_CreateTextureFromSurface(gameEngine->renderer, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(levelTTF, NULL,NULL, &levelTTFRect.w, &levelTTFRect.h);
    levelTTFRect.x = 100;
    levelTTFRect.y = 100;


    tmp = TTF_RenderText_Solid(returnFont, game->level->getLevelName().c_str(), whiteColor);
    levelNameTTF = SDL_CreateTextureFromSurface(gameEngine->renderer, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(levelNameTTF, NULL,NULL, &levelNameTTFRect.w, &levelNameTTFRect.h);
    levelNameTTFRect.x = 150;
    levelNameTTFRect.y = 150;


}

void GameState::destroy(){
    delete game;
    game = nullptr;
}

void GameState::pause(){

}

void GameState::resume(){

}

void GameState::saveReplay(){

    if(game != NULL){
        game->saveReplay();
    }

}


void GameState::handleInput(GameEngine* gameEngine){
    if(SDL_GetTicks()-time > 2000){
       game->handleInput();
    }
}

void GameState::update(GameEngine* gameEngine){
    if(SDL_GetTicks()-time > 2000){
       game->update();
    }
}


void GameState::render(GameEngine* gameEngine){
    SDL_SetRenderDrawColor(gameEngine->renderer, 0,0,0,255);
    SDL_RenderClear(gameEngine->renderer);
    if(SDL_GetTicks()-time > 2000){
       game->render();
    }else{
        SDL_RenderCopy(gameEngine->renderer, levelTTF, NULL, &levelTTFRect);
        SDL_RenderCopy(gameEngine->renderer, levelNameTTF, NULL, &levelNameTTFRect);
    }
    SDL_RenderPresent(gameEngine->renderer);
}
