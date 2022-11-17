#pragma once

#include "SDL.h"
#include "State.h"
#include <vector>
#include <stack>
#include <States/State.h>
#include <Game.h>

class GameState : public State{
public:
  void init(GameEngine* gameEngine);
  void destroy();

  void pause();
  void resume();

  void handleInput(GameEngine* game);
  void update(GameEngine* game);
  void render(GameEngine* game);

  void saveReplay();


    static GameState* getInstance(){
        static GameState instance;
        return &instance;
    }

protected:
private:
    GameState() {}
    GameState(GameState const&);
    void operator=(GameState const&);

    SDL_Texture* levelTTF;
    SDL_Texture* levelNameTTF;

    SDL_Rect levelTTFRect;
    SDL_Rect levelNameTTFRect;

    uint32_t time;

    Game* game = nullptr;

};

