#pragma once

#include <GameEngine.h>

class State {
   public:
    virtual void init(GameEngine* gameEngine) = 0;
    virtual void destroy() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void handleInput(GameEngine* game) = 0;
    virtual void update(GameEngine* game) = 0;
    virtual void render(GameEngine* game) = 0;

    void changeState(GameEngine* game, State* state) {
        game->changeState(state);
    }

   protected:
    State() {}
};
