#pragma once

#include <Entity.h>
#include <Enums.h>
#include <Vector2D.h>
#include <Level.h>

#include <SparkleAnimation.h>

class Door : public Entity
{
    public:
        Door(Game* g, float x, float y, float w, float h);
        Door(){};
        ~Door();

        void checkCollision(Level* level);
        void checkPlayerCollision();

        void update();
        void render(Camera*);

    protected:
        SparkleAnimation sparkleAni;

    private:
};

