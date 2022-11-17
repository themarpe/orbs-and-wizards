#pragma once

#include <SDL.h>
#include <State.h>
#include <MainMenuState.h>

class GameWonState : public State{

public:
    void init(GameEngine* gameEngine);
    void destroy();
    void pause();
    void resume();
    void handleInput(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static GameWonState* getInstance(){
        static GameWonState instance;
        return &instance;
    }

protected:
private:
    SDL_Renderer* renderer;

    SDL_Texture* gamescreen;
    SDL_Texture* pointer;

    SDL_Rect pointerRect;

    int pointerPos;

    GameWonState() {}
    GameWonState(GameWonState const&);
    void operator=(GameWonState const&);

};
