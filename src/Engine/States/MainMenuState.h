#pragma once

#include <SDL.h>
#include <State.h>

#include <windows.h>
#include <LevelCreatorSelector.h>

class MainMenuState : public State{

public:
    void init(GameEngine* gameEngine);
    void destroy();
    void pause();
    void resume();
    void handleInput(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static MainMenuState* getInstance(){
        static MainMenuState instance;
        return &instance;
    }

protected:
private:
    SDL_Renderer* renderer;

    SDL_Texture* mainscreen;
    SDL_Texture* pointer;

    SDL_Rect pointerRect;

    int pointerPos;

    MainMenuState() {}
    MainMenuState(MainMenuState const&);
    void operator=(MainMenuState const&);

};
