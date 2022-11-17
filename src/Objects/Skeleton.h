#ifndef SKELETON_H
#define SKELETON_H

#include <Enums.h>
#include <HittableEnemy.h>
#include <Potion.h>
#include <Rune.h>
#include <RuneKey.h>
#include <Vector2D.h>
#include <stdlib.h>

#include <map>

class Camera;
class Game;
class Level;
class SpriteSheet;
class Player;

class Skeleton : public HittableEnemy {
   public:
    enum class Stance { STANDING, RUNNING, FALLING, ATTACK, DEAD, SPAWN, HIT, FROZEN };

    Skeleton(Game* g, std::string key, float x, float y, float w, float h);
    Skeleton();
    ~Skeleton();

    void checkCollision(Level* level);
    void checkPlayerCollision();

    bool getHittable();
    bool getFrezze();

    void frezze(int);

    void setSpeed(float, float);

    void land();
    void setStance(Stance, Direction);

    void hit(int damage);
    void hit(int damage, Direction);

    int getHealthPoints();
    void setHealthPoints(int);

    void drop();

    void update();
    void render(Camera*);

   protected:
    uint32_t aniSpeed;

    uint32_t time;

    int speed;

    int frozenDuration;

    int hitDuration;

    Vector2D dir, accel;
    Stance stance;
    Direction direction;

    Direction dirHit;

   private:
};

#endif  // SKELETON_H
