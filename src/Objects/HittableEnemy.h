#ifndef HITTABLEENEMY_H
#define HITTABLEENEMY_H

#include <Entity.h>
#include <Enums.h>
class Game;

class HittableEnemy : public Entity
{
    public:
        HittableEnemy();
        HittableEnemy(Game*, std::string, float, float, float, float);
        ~HittableEnemy();

        virtual void hit(int damage);

        virtual bool getHittable() = 0;
        virtual void hit(int damaga, Direction direction) = 0;

        virtual bool getFrezze() = 0;
        virtual void frezze(int duration) = 0;

    protected:
        int healthPoints;

    private:
};

#endif // HITTABLEENEMY_H
