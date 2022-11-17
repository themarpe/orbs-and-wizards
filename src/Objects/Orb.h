#pragma once //HEADERGUARD


#include "SDL.h"
#include <iostream>
#include "Entity.h"
#include "../Engine/Vector2D.h"

#include <Skeleton.h>
class Camera;
class Game;
class SpriteSheet;
class Level;


class Orb : public Entity
{
    public:
        enum class Stance{START, END};
        Orb(Game*, std::string, float, float, float, float);
        ~Orb();
        void update();
        void render(Camera*);
        void shoot(int, float, int ,float, float);
        static Uint32 nextSpriteCallback(Uint32, void*);
        SDL_TimerID timerId;

        SDL_Rect collisionBox;

        SDL_Rect* getCollisionRect();

        void checkCollisionLevel(Level* level);
        void checkCollisionHittableEnemy();

        void hit();

    protected:
        Vector2D dir, accel;

        float angle;
        int power;
        int frezze;
        int startFlyingSprites, endFlyingSprites, numSprites;

        int orb;

        uint32_t time;

        Stance orbStance;

    private:

};

