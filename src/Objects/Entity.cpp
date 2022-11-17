#include "Entity.h"

#include "../Engine/Game.h"

Entity::Entity(){
    currentSprite = 0;
    alive = 1;
}

Entity::~Entity(){

}

Entity::Entity(Game* g, std::string key, float x, float y, float w, float h){
    game_p = g;
    spriteSheetKey = key;
    currentSprite = 0;
    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    alive = 1;
    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;

    collisionRect.x = x;
    collisionRect.y = y;
    collisionRect.w = w;
    collisionRect.h = h;

}

void Entity::setX(int x)
{
    this->x = x;
}
void Entity::setY(int y)
{
    this->y = y;
}

void Entity::setW(int w)
{
    this->w = w;
}
void Entity::setH(int h)
{
    this->h = h;
}

int Entity::getX()
{
    return x;
}

int Entity::getY()
{
    return y;
}

int Entity::getW()
{
    return w;
}

int Entity::getH()
{
    return h;
}

void Entity::setCurrentSprite(int cS)
{
    currentSprite = cS;
}

int Entity::getCurrentSprite()
{
    return currentSprite;
}

void Entity::setAlive(bool a)
{
    alive = a;
}

bool Entity::getAlive()
{
    return alive;
}

SDL_Rect* Entity::getRenderRect(){
    return &renderRect;
}

SDL_Rect* Entity::getCollisionRect(){
    return &collisionRect;
}


