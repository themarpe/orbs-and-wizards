#include <iostream>

// Using SDL and standard IO
#include <GameEngine.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <States/GameState.h>
#include <States/MainMenuState.h>

#include <chrono>
#include <cstdio>
#include <thread>

int main(int argc, char* args[]) {
    using namespace std::chrono;

    GameEngine gameEngine;

    gameEngine.init();
    gameEngine.setLevelPath("Levels/level00.lvl");
    gameEngine.pushState(MainMenuState::getInstance());

    // Set FPS limit
    constexpr float FPS = 60;
    constexpr auto FRAME_TIME = 1s / FPS;
    while(gameEngine.running) {
        auto t1 = steady_clock::now();
        gameEngine.handleInput();
        gameEngine.update();
        gameEngine.render();
        auto t2 = steady_clock::now();
        auto elapsedTime = t2 - t1;
        auto remainingTime = FRAME_TIME - elapsedTime;
        if(remainingTime > 0ms) {
            std::this_thread::sleep_for(remainingTime);
        }
    }

    gameEngine.destroy();

    return 0;
}
