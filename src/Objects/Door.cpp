#include "Door.h"
#include "../Engine/Game.h"
#include "../Engine/Camera.h"
#include "../Engine/Level.h"
#include "../Engine/SpriteSheet.h"
#include "Block.h"
#include <Player.h>
#include <Entity.h>


Door::Door(Game* g, float x, float y, float w, float h) : Entity(g, "Door", x, y, w ,h){

    currentSprite = 0;

    collisionRect.x = x;
    collisionRect.y = y;

    sparkleAni.setSpriteSheetKey("ManaAnimation");
    sparkleAni.setX(x-w/2);
    sparkleAni.setY(y-h/2);
    sparkleAni.setW(w+w);
    sparkleAni.setH(h*2);

}

Door::~Door()
{
    //dtor
}


void Door::update(){
    checkPlayerCollision();
    sparkleAni.update();
}


void Door::render(Camera* camera){

    renderRect.x = x;
    renderRect.y = y;

    //DEBUG
    //camera->RenderDrawRect(&collisionRect);

    camera->RenderCopy(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), Game::SPRITESHEET.at(spriteSheetKey).getSpriteRectAt(currentSprite), &renderRect);
    sparkleAni.render(camera);

}


void Door::checkPlayerCollision(){
    if(Game::checkBoxCollision(game_p->p1->getCollisionRect(), getCollisionRect())){
        Player* p = game_p->p1;
        if(p->getRuneKey()>=3){
            if(p->keys[(int)Player::Keys::E] && !sparkleAni.getPlaying()){
                std::cout << "WWOOOON!\n";
                sparkleAni.startAnimation();
                game_p->nextLevel();
            }
        }
    }
}

