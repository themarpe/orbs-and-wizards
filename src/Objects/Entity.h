#pragma once  // HEADERGUARD

#include <GameEngine.h>

#include <iostream>

#include "SDL.h"

class Game;
class Camera;

class Entity {
   protected:
    Game* game_p;
    float x, y, w, h;
    int currentSprite;
    SDL_Rect renderRect;
    SDL_Rect collisionRect;
    bool alive;
    std::string spriteSheetKey;

   public:
    Entity();
    Entity(Game* g, std::string key, float x, float y, float w, float h);
    virtual ~Entity();
    virtual void update() = 0;
    virtual void render(Camera*) = 0;
    void setX(int x);
    void setY(int y);
    void setW(int w);
    void setH(int h);
    int getX();
    int getY();
    int getW();
    int getH();
    void setCurrentSprite(int cS);
    int getCurrentSprite();
    void setAlive(bool a);
    bool getAlive();

    SDL_Rect* getRenderRect();
    SDL_Rect* getCollisionRect();
};
