#ifndef TURRET_H
#define TURRET_H

#include <Entity.h>

class Game;
class Camera;
class Player;
class Turret : public Entity
{
    public:
        Turret(Game* g, std::string key, float x, float y, float w, float h, uint32_t);
        ~Turret();

        void update();
        void render(Camera*);
        void locatePlayer(Player*);

    protected:
        uint32_t time;
        uint32_t speed;


    private:
};

#endif // TURRET_H
