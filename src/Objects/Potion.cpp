#define NOMINMAX

#include "Potion.h"

#include "../Engine/Game.h"
#include "../Engine/Camera.h"
#include "../Engine/Level.h"
#include "../Engine/SpriteSheet.h"
#include "Block.h"
#include <Player.h>
#include <Entity.h>
#include <algorithm>

Potion::Potion(Game* g, float x, float y, float w, float h, PotionType p) : Entity(g, "", x, y, w, h){


    dir.x = 0;
    dir.y = -1;
    accel.x = 0;
    accel.y = 0.2;

    time = SDL_GetTicks();
    timeDropped = SDL_GetTicks();

        potionType = p;
    if(p == PotionType::HEALTH){
        spriteSheetKey = "HealthPotion";
    }else if(p == PotionType::MANA){
        spriteSheetKey = "ManaPotion";
    }

}

Potion::~Potion()
{
    std::cout << "killed mana = " << this << "\n";
}



void Potion::checkCollision(Level* level){

    std::vector<Block*> collideNeighbours;
    std::pair<int, int> corners[6];

    corners[0] = std::make_pair(x/level->getTileSize(), y/level->getTileSize());
    corners[1] = std::make_pair(x/level->getTileSize(), (y+h/2)/level->getTileSize());
    corners[2] = std::make_pair(x/level->getTileSize(), (y+h)/level->getTileSize());
    corners[3] = std::make_pair((x+w)/level->getTileSize(), (y+h)/level->getTileSize());
    corners[4] = std::make_pair(x+w/level->getTileSize(), (y+h/2)/level->getTileSize());
    corners[5] = std::make_pair((x+w)/level->getTileSize(), (y)/level->getTileSize());

    for(int c = 0; c<6; c++){
        int32_t i = corners[c].second, j=corners[c].first;
        if(j >= 0 && j < level->getLevelWidth()/level->getTileSize() &&
            i >= 0 && i < level->getLevelHeight()/level->getTileSize()){
                if(level->levelData[i][j] != NULL && level->levelData[i][j]->getCollidable()){
                        collideNeighbours.push_back(level->levelData[i][j]);
                }
        }
    }

    for(int c = 0; c<collideNeighbours.size(); c++){

        Vector2D tilePos(collideNeighbours[c]->getX() + collideNeighbours[c]->getW()/2.0, collideNeighbours[c]->getY() + collideNeighbours[c]->getH()/2.0);
        const float MAX_DISTANCEX = w/2.0 + collideNeighbours[c]->getW()/2.0;
        const float MAX_DISTANCEY = h/2.0 + collideNeighbours[c]->getH()/2.0;
        Vector2D centerPlayerPos = Vector2D(x + w/2.0 , y + h/2.0);
        Vector2D distVec = centerPlayerPos - tilePos;

        float xDepth = MAX_DISTANCEX - std::abs(distVec.x);
        float yDepth = MAX_DISTANCEY - std::abs(distVec.y);

        if (xDepth > 0 && yDepth > 0){
            if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
                if (distVec.x > 0) {
                    x += xDepth-1;
                }else{
                    x -= xDepth-1;
                }
            }else {
                if(distVec.y > 0){
                   y += yDepth -1;
                   dir.y = 0.1;
                }else{
                    y -= yDepth-1;
                    dir.y = 0;
                }
            }
        }
    }

}


void Potion::checkPlayerCollision(){
    if(SDL_GetTicks() - timeDropped > 500){
        if(Game::checkBoxCollision(game_p->p1->getCollisionRect(), getCollisionRect())){
           if(potionType == PotionType::HEALTH){
                game_p->p1->healAnimationStart();
                game_p->p1->setHealthPoints(game_p->p1->getHealthPoints()+2);
           }else if(potionType == PotionType::MANA){
                game_p->p1->manaAnimationStart();
                game_p->p1->setMana(game_p->p1->getMana()+20);
           }

            setAlive(0);

        }
    }
}



void Potion::update(){

    collisionRect.x = x;
    collisionRect.y = y;

    dir.x += accel.x;
    dir.y += accel.y;
    x += dir.x;
    y += dir.y;

    checkCollision(game_p->level);
    checkPlayerCollision();

    if(SDL_GetTicks() - time > 125){
        if(++currentSprite >= 8) currentSprite = 0;
        time = SDL_GetTicks();
    }


}


void Potion::render(Camera* camera){

    renderRect.x = x;
    renderRect.y = y;

    //DEBUG
    //camera->RenderDrawRect(&collisionRect);

    camera->RenderCopy(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), Game::SPRITESHEET.at(spriteSheetKey).getSpriteRectAt(currentSprite), &renderRect);

}
