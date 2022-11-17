#pragma once //HEADERGUARD

#include "SDL.h"

#include "Entity.h"
#include <GameEngine.h>

class Camera;
class Game;

class Block : public Entity{
    public:
        Block(Game*, std::string, float, float, float, float, bool);
        void update();
        void render(Camera*);
        bool getCollidable();
        void setCollidable(bool);
    private:
        bool collidable;
};

