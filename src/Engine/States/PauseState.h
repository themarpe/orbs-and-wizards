#pragma once

#include <SDL.h>
#include <State.h>

class PauseState : public State {
   public:
    void init(GameEngine* gameEngine);
    void destroy();
    void pause();
    void resume();
    void handleInput(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static PauseState* getInstance() {
        static PauseState instance;
        return &instance;
    }

   protected:
   private:
    SDL_Renderer* renderer;

    SDL_Texture* pausescreen;
    SDL_Texture* pointer;

    SDL_Rect pointerRect;

    int pointerPos = 0;

    PauseState() {}
    PauseState(PauseState const&);
    void operator=(PauseState const&);
};
