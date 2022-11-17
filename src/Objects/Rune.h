#ifndef RUNE_H
#define RUNE_H

#include <Entity.h>
#include <Enums.h>
#include <Vector2D.h>
#include <Level.h>

class Rune : public Entity
{
    public:
        Rune(Game* g, float x, float y, float w, float h, RuneType p);
        Rune(){};
        ~Rune();

        void checkCollision(Level* level);
        void checkPlayerCollision();

        void update();
        void render(Camera*);

    protected:

    RuneType runeType;
    Vector2D dir, accel;

    uint32_t timeDropped;
    private:
};

#endif // RUNE_H
