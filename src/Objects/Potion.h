#ifndef MANA_H
#define MANA_H

#include <Entity.h>

#include <Vector2D.h>
#include <Enums.h>
#include <Level.h>

class Potion : public Entity
{
    public:
        Potion(Game* g, float x, float y, float w, float h, PotionType p);
        Potion(){};
        ~Potion();

        void checkCollision(Level* level);
        void checkPlayerCollision();

        void update();
        void render(Camera*);

    protected:

    PotionType potionType;
    Vector2D dir, accel;

    uint32_t time;
    uint32_t timeDropped;

    private:
};

#endif // MANA_H
