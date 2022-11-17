#include<iostream>

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <GameEngine.h>
#include <States/GameState.h>
#include <States/MainMenuState.h>


int main( int argc, char* args[] )
{

    GameEngine gameEngine;

    gameEngine.init();
    gameEngine.setLevelPath("Levels/level00.lvl");
    gameEngine.pushState(MainMenuState::getInstance());

    while(gameEngine.running){
        gameEngine.handleInput();
        gameEngine.update();
        gameEngine.render();
    }

    gameEngine.destroy();

    return 0;
}
