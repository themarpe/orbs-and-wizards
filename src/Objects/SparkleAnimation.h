#pragma once //HEADERGUARD

#include <SDL.h>
#include <iostream>

#include "Entity.h"


class Camera;
class Game;

class SparkleAnimation : public Entity{
public:
    SparkleAnimation();
    ~SparkleAnimation();

    void update();
    void render(Camera*);

    bool getPlaying(){
        return (currentSprite>-1) ? true : false;
    }

    void startAnimation();

    void setSpriteSheetKey(std::string key){
        spriteSheetKey = key;
    }

protected:
private:
    uint32_t time;

};


