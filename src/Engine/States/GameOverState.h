#pragma once

#include <MainMenuState.h>
#include <SDL.h>
#include <State.h>

class GameOverState : public State {
   public:
    void init(GameEngine* gameEngine);
    void destroy();
    void pause();
    void resume();
    void handleInput(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static GameOverState* getInstance() {
        static GameOverState instance;
        return &instance;
    }

   protected:
   private:
    SDL_Renderer* renderer;

    SDL_Texture* gamescreen;
    SDL_Texture* pointer;

    SDL_Rect pointerRect;

    int pointerPos;

    GameOverState() {}
    GameOverState(GameOverState const&);
    void operator=(GameOverState const&);
};
