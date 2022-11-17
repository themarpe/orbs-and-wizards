#include "Orb.h"

#include "../Engine/Game.h"
#include "../Engine/SpriteSheet.h"
#include "../Engine/Camera.h"
#include "../Engine/Level.h"
#include "Block.h"

#define PI 3.14159265

Orb::Orb(Game* g, std::string key, float x , float y, float w , float h) : Entity(g, key, x, y, w, h)
{
    dir.Set(0,0);
    accel.Set(0,0);
    power = 0;
    startFlyingSprites = 8;
    endFlyingSprites = 44;
    numSprites = 56;

    collisionBox.x = x+w/3;
    collisionBox.y = y+h/3;

    collisionBox.w = w-w/2;
    collisionBox.h = h-h/2;

    time = SDL_GetTicks();
}

Orb::~Orb(){

}


void Orb::checkCollisionLevel(Level* level){
    std::pair<int, int> point;
    point = std::make_pair((x+w/2.0) / level->getTileSize(), (y+h/2.0) / level->getTileSize());
    if(point.first >= 0 && point.first < level->getLevelWidth()/level->getTileSize()
        && point.second >= 0 && point.second < level->getLevelHeight() / level->getTileSize()){
       if(level->levelData[point.second][point.first] != NULL && level->levelData[point.second][point.first]->getCollidable()){
            //std::cout << "COLLISION BULLET VS LEVEL \n";
            if(orbStance != Stance::END) hit();
       }
    }
}

void Orb::hit(){
    orbStance = Stance::END;
    currentSprite = endFlyingSprites;
}

SDL_Rect* Orb::getCollisionRect(){
    return &collisionBox;
}

void Orb::checkCollisionHittableEnemy(){
    for(auto e : game_p->entities){
        if(orbStance != Stance::END){
            if(Game::checkBoxCollision(e->getCollisionRect(), &collisionBox)){
                HittableEnemy* hittableEnemy = dynamic_cast<HittableEnemy*>(e);
                if(hittableEnemy != NULL && hittableEnemy->getHittable()){
                    if(frezze && hittableEnemy->getFrezze()){
                        hittableEnemy->hit(power);
                        if(!(orb == 2 || orb == 5) && orbStance != Stance::END)  hit();
                        hittableEnemy->frezze(frezze);
                    }else{
                        if(dir.x > 0) hittableEnemy->hit(power, Direction::RIGHT);
                        else hittableEnemy->hit(power, Direction::LEFT);
                        if(!(orb == 2 || orb == 5) && orbStance != Stance::END)  hit();
                    }
                }
            }
        }
    }
}

void Orb::update()
{

    collisionBox.x = x+w/3;
    collisionBox.y = y+h/3;

    if(!(orb == 2 || orb == 5)) checkCollisionLevel(game_p->level);

    checkCollisionHittableEnemy();


    if(currentSprite>=startFlyingSprites && currentSprite<endFlyingSprites){
        x += dir.x;
        y += dir.y;
        dir.Set(dir.x+accel.x, dir.y+accel.y);
    }

    if(x + w < 0 || x > game_p->level->getLevelWidth() || y + h < 0 || y > game_p->level->getLevelHeight()) setAlive(0);

    if(SDL_GetTicks() - time > 40){
        currentSprite++;
        if(orbStance == Stance::START){
            if(currentSprite >= endFlyingSprites){
                currentSprite = startFlyingSprites;
            }
        }else{
            if( currentSprite >= numSprites){
                currentSprite = 0;
                setAlive(0);
            }
        }

        time = SDL_GetTicks();
    }


}

void Orb::shoot(int orb, float power, int frezze, float speed, float angle){
    this->orb = orb;
    this->angle = angle;
    this->power = power;
    this->frezze = frezze;

    orbStance = Stance::START;

    //param*PI/180
    dir.Set(cos(angle/180 * PI)*speed, sin(angle/180 * PI) * speed);

    //std::cout << "dir.x = " << dir.x << " dir.y = " << dir.y << " angle = " << angle << "\n";

    if(currentSprite != 0) return;
    currentSprite = 0;
}


void Orb::render(Camera* camera){
    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    camera->RenderCopyEx(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), Game::SPRITESHEET.at(spriteSheetKey).getSpriteRectAt(currentSprite), &renderRect, 270+angle, 0, flip);


    collisionBox.x = x + w/2;
    collisionBox.y = y + h/2 - h/8;

    collisionBox.w = 10;
    collisionBox.h = h/4;

  //  std::cout << "cb.x = " << collisionBox.x << " cb.y = " << collisionBox.y << " cb.w = " << collisionBox.w << " cb.h = " << collisionBox.h << "\n";

    //DEBUG
    //camera->RenderDrawRect(&collisionBox);
}

