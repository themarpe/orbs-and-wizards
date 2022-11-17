#pragma once

#include <Entity.h>
#include <Enums.h>
#include <Level.h>
#include <Vector2D.h>

class RuneKey : public Entity {
   public:
    RuneKey(Game* g, float x, float y, float w, float h);
    RuneKey(){};
    ~RuneKey();

    void checkCollision(Level* level);
    void checkPlayerCollision();

    void update();
    void render(Camera*);

   protected:
    Vector2D dir, accel;

    uint32_t timeDropped;

   private:
};
