#pragma once //HEADERGUARD


#include <iostream>
#include <iomanip>      // std::setprecision

#include <math.h>
#include <SDL.h>

#include "Entity.h"
#include "../Engine/Vector2D.h"

#include <Enums.h>

class Camera;
class Orb;
class SparkleAnimation;
class Game;
class Level;
class Block;
class SpriteSheet;

class Player : public Entity
{

public:
    enum class Keys{LEFT = 0, RIGHT = 1, UP = 2, SPACE = 3, NUM1 = 4, NUM2 = 5, NUM3 = 6, Q = 7, E = 8};

    enum class Stance{STANDING, RUNNING, DEAD, FALLING, ATTACK, HIT};
    Stance playerStance;
    Direction playerDirection;


public:
    Player(Game*, std::string, float, float, float, float, int);
    ~Player();
    SparkleAnimation* sparkleAni;


    void land();

    void hit(int, Direction);

    void setDirection(Direction);
    void setStance(Stance);
    void startJumping();
    void stopJumping();

    void checkCollisionHittableEnemy();

    SDL_Rect* getCollisionBox();

    void setCurrentOrb(int);
    int getCurrentOrb();

    void attack();
    void specialAttack();

    void startAttacking();
    void stopAttacking();
    void startMovingLeft();
    void startMovingRight();
    void stopMovingLeft();
    void stopMovingRight();

    int getHealthPoints();
    void setHealthPoints(int);

    int getMana(){
        return mana;
    }
    void setMana(int m){
        if(m > 100) mana = 100;
        else mana = m;
    }

    void healAnimationStart();
    void manaAnimationStart();
    void runeAnimationStart();

    void checkCollision(Level*);

    void update();
    void render(Camera*);

    void handleInput();

    bool keys[9];

    int getRuneKey(){
        return runeKey;
    }

    void setRuneKey(int k){
        if(k>3) runeKey = 3;
        else runeKey = k;
    }

    int getOrbInv(int index){
        return orbInv[index];
    }

    void setOrbInv(int index, int amount){
        if(amount > 99) orbInv[index] = 99;
        else orbInv[index] = amount;
    }


protected:

    Vector2D dir, accel;

    std::string orbs[6] = {"FireOrb2", "FireOrb2", "FireOrb2", "IceOrb3", "IceOrb3", "IceOrb2"};
    int orbPower[6] = {2,5,20, 1,1,1};
    int orbFrezze[6] = {0,0,0, 60, 180, 300};

    float orbSpeed[6] = {8.5,7.0,5.0, 8.5,7.0,5.0};
    int orbSize[6] = {64,96,256,64,96,192};
    int orbMana[6] = {10,20,50,10,20,50};

    int orbInv[6] = {-1, 0, 0, -1, 0, 0};

    int currentFireOrb = 0;
    int currentIceOrb = 3;

    bool keyQ = false;
    bool keyE = false;

    int currentOrb = -1;

    bool castOrb(int);

    uint32_t time;
    uint32_t hitTime;

    int runeKey = 0;

    int mana;
    int healthPoints;

    SDL_Rect staffCollision;

    SDL_Rect collisionBox;


};


