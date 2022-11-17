#define NOMINMAX

#include "Player.h"

#include "../Engine/Game.h"
#include "Orb.h"
#include "SparkleAnimation.h"
#include "../Engine/Level.h"
#include "Block.h"
#include "../Engine/SpriteSheet.h"
#include "../Engine/Camera.h"
#include "Skeleton.h"
#include <UI.h>
#include <algorithm>


Player::Player(Game* game, std::string sheet, float x, float y, float w, float h, int health) : Entity(game, sheet, x, y, w, h){
    sparkleAni = new SparkleAnimation;

    sparkleAni->setW(w*(3.0/4.0));
    sparkleAni->setH(h*(3.0/4.0));

    healthPoints = health;
    dir.Set(0,0);
    accel.Set(0,0.2);

    currentOrb = 0;
    time = SDL_GetTicks();

    hitTime = SDL_GetTicks();

    for(int i=0; i<9; i++) keys[i] = false;

    currentOrb = -1;
    currentFireOrb = 0;
    currentIceOrb = 0;

    mana = 100;
    runeKey = 0;

}


Player::~Player(){
    delete sparkleAni;
}

void Player::checkCollision(Level* level){

    std::vector<Block*> collideNeighbours;
    std::pair<int, int> corners[8];
    corners[0] = std::make_pair(collisionRect.x/level->getTileSize(), collisionRect.y/level->getTileSize());
    corners[1] = std::make_pair(collisionRect.x/level->getTileSize(), (collisionRect.y+collisionRect.h/2.0)/level->getTileSize());
    corners[2] = std::make_pair(collisionRect.x/level->getTileSize(), (collisionRect.y+collisionRect.h) / level->getTileSize());
    corners[3] = std::make_pair((collisionRect.x+collisionRect.w/2.0)/level->getTileSize(), (collisionRect.y+collisionRect.h) / level->getTileSize());
    corners[4] = std::make_pair((collisionRect.x+collisionRect.w)/level->getTileSize(), (collisionRect.y+collisionRect.h)/level->getTileSize());
    corners[5] = std::make_pair((collisionRect.x+collisionRect.w)/level->getTileSize(), (collisionRect.y+collisionRect.h/2.0)/level->getTileSize());
    corners[6] = std::make_pair((collisionRect.x+collisionRect.w)/level->getTileSize(), collisionRect.y/level->getTileSize());
    corners[7] = std::make_pair((collisionRect.x+collisionRect.w/2.0)/level->getTileSize(), collisionRect.y/level->getTileSize());

    for(int c = 0; c<8; c++){
        int32_t i = corners[c].second, j=corners[c].first;
        if(j >= 0 && j < level->getLevelWidth()/32 &&
            i >= 0 && i < level->getLevelHeight()/32 &&
                level->levelData[i][j] != NULL &&
                    level->levelData[i][j]->getCollidable())
                        collideNeighbours.push_back(level->levelData[i][j]);
    }

    for(int c = 0; c<collideNeighbours.size(); c++){

        Vector2D tilePos(collideNeighbours[c]->getX() + collideNeighbours[c]->getW()/2.0, collideNeighbours[c]->getY() + collideNeighbours[c]->getH()/2.0);
        const float MAX_DISTANCEX = collisionRect.w/2.0 + collideNeighbours[c]->getW()/2.0;
        const float MAX_DISTANCEY = collisionRect.h/2.0 + collideNeighbours[c]->getH()/2.0;
        Vector2D centerPlayerPos = Vector2D(collisionRect.x + collisionRect.w/2.0 , collisionRect.y + collisionRect.h/2.0);
        Vector2D distVec = centerPlayerPos - tilePos;

      //  std::cout << c <<" dvx = " << distVec.x << " dvy = " << distVec.y << "\n";

        float xDepth = MAX_DISTANCEX - std::abs(distVec.x) - 2;
        float yDepth = MAX_DISTANCEY - std::abs(distVec.y) - 2;

        if (xDepth > 0 && yDepth > 0){
            if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
                if (distVec.x > 0) {
                    x += xDepth-1;
                    dir.Set(0.1, dir.y);
                }else{
                    x -= xDepth-1;
                    dir.Set(-0.1, dir.y);
                }
            }else {
                if(distVec.y > 0){
                   y += yDepth-1;
                   dir.Set(dir.x, 0.1);
                }else{
                    y -= yDepth-1;
                    land();
                }
            }
        }
    }


}

void Player::checkCollisionHittableEnemy(){
    if(currentOrb == -1){
        staffCollision = collisionRect;
        if(playerStance == Stance::ATTACK){
            if(playerDirection == Direction::LEFT){
                staffCollision.x = x;
                staffCollision.y = staffCollision.y+staffCollision.h/2;
                staffCollision.w = collisionRect.w;
                staffCollision.h = (4 * h/32);
            }else{
                staffCollision.x = collisionRect.x + collisionRect.w;
                staffCollision.y = staffCollision.y+staffCollision.h/2;
                staffCollision.w = (10 * w/32);
                staffCollision.h = (4 * h/32);
            }
        }
        for(int i = 0; i<game_p->entities.size(); i++){
            HittableEnemy* hittableEnemy = dynamic_cast<HittableEnemy*>(game_p->entities[i]);
            if(hittableEnemy != NULL && hittableEnemy->getHittable() && Game::checkBoxCollision(&staffCollision, hittableEnemy->getCollisionRect() )){
                if(playerStance == Stance::ATTACK){
                    hittableEnemy->hit(1, playerDirection);
                }
            }
        }
    }

}


void Player::handleInput(){

    if(alive && playerStance != Stance::HIT){

        if(keys[(int)Keys::LEFT]){
            if(playerStance != Stance::ATTACK || currentOrb == -1){
                setDirection(Direction::LEFT);
                if(playerStance == Stance::STANDING && playerStance != Stance::ATTACK){
                    setStance(Stance::RUNNING);
                }

                dir.Set(-2.5, dir.y);

            }
        }
        if(keys[(int)Keys::RIGHT]){

            if(playerStance != Stance::ATTACK || currentOrb == -1){
                setDirection(Direction::RIGHT);
                if(playerStance == Stance::STANDING && playerStance != Stance::ATTACK){
                    setStance(Stance::RUNNING);
                }
                dir.Set(2.5, dir.y);

            }
        }
        if(!(keys[(int)Keys::LEFT] || keys[(int)Keys::RIGHT])){
            if(playerStance == Stance::RUNNING){
                dir.Set(0, dir.y);
                setStance(Stance::STANDING);
            }
        }

        if(!keys[(int)Keys::LEFT] && dir.x < 0 && playerStance != Stance::HIT) dir.Set(0, dir.y);
        if(!keys[(int)Keys::RIGHT] && dir.x >= 0 && playerStance != Stance::HIT) dir.Set(0, dir.y);


        if(keys[(int)Keys::UP]){
            if(playerStance != Stance::FALLING && dir.y == 0){
                y-=2;
                dir.Set(dir.x, -5.5);
                if(playerStance != Stance::ATTACK) setStance(Stance::FALLING);
            }
        }
        if(keys[(int)Keys::SPACE]){
            attack();
        }

        if(keys[(int)Keys::NUM1]){
            currentOrb = -1;
        }else if(keys[(int)Keys::NUM2]){
            currentOrb = currentFireOrb;

        }else if(keys[(int)Keys::NUM3]){
            currentOrb = currentIceOrb+3;
        }

        if(keys[(int)Keys::Q] && keyQ == false){
            if(currentOrb >= 0 && currentOrb < 3){
                currentFireOrb = (++currentFireOrb)%3;
                currentOrb = currentFireOrb;
            }else if(currentOrb >= 3 && currentOrb < 6){
                currentIceOrb = (++currentIceOrb)%3;
                currentOrb = currentIceOrb+3;
            }
            keyQ = true;
        }

        if(!keys[(int)Keys::Q]) keyQ = false;

         //std::cout << "currentOrb = " << currentOrb << " ice curr = " << currentIceOrb << " curr fire = " << currentFireOrb<< "\n";

    }

}


void Player::update()
{

    sparkleAni->update();

   // std::cout << "begin dir.x = " << healthPoints << " dir.y = " << dir.y << "\n";

    //kill animation if player is dead
    if(healthPoints <= 0){
        if(playerStance != Stance::DEAD){
            dir.Set(0, -2.0);
            setStance(Stance::DEAD);
            //setAlive(0);
        }
    }

    if(y > 2000){
        setAlive(0);
    }

    if(playerStance == Stance::HIT){
        if(SDL_GetTicks() - hitTime > 500){
            setStance(Stance::STANDING);
        }
    }

    //set to running stance if moving
    if(dir.x != 0 && playerStance == Stance::STANDING) setStance(Stance::RUNNING);

    //SET HEALANIMATION TO SAME X & Y and then update
    sparkleAni->setX(x+(w/16));
    sparkleAni->setY(y+(h/4));


    if(SDL_GetTicks() - time > 100){
        switch(playerStance){
            case Stance::STANDING:
                if(++currentSprite >= 4) currentSprite = 0;
                break;
            case Stance::RUNNING:
                if(++currentSprite >= 12) currentSprite = 6;
                break;

            case Stance::FALLING:
                if(++currentSprite >= 13) currentSprite = 12;
                break;

            case Stance::ATTACK:
                if(++currentSprite >= 22){
                    setStance(Stance::STANDING);
                    currentSprite = 18;
                }
                break;

            case Stance::DEAD:
                SDL_SetTextureColorMod(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), 255,255,255);
                break;

            case Stance::HIT:
                SDL_SetTextureColorMod(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), 255,255,255);
                break;

            default:
                break;
        }
        time = SDL_GetTicks();
    }

    //RESET PLAYER COLLISION BOXES
    collisionRect.x = x+(9 * w/32);
    collisionRect.y = y+(14 * h/32);
    collisionRect.w = 12 * w/32;
    collisionRect.h = 16 * h/32;


    dir.y += accel.y;  //ACCELERATE IN DIRECTION OF GRAVITY

    //CHECK FOR COLLISIONS if alive
    if(playerStance != Stance::DEAD){
        checkCollision(game_p->level);
        checkCollisionHittableEnemy();
    }

    //move player for amout of speed in x and y
    x += dir.x; //MOVE x
    y += dir.y; //MOVE y

    //SET CAMERA
    if(x - game_p->screenWidth/2 >= 0 && (x + game_p->screenWidth/2) < game_p->level->getLevelWidth()){
        game_p->camera->setX(x - game_p->screenWidth/2);
    }

    //UI update
        if(currentOrb == -1){
                game_p->ui->setWeapon(-1);
        }else{
            if(currentOrb < 3){
                game_p->ui->setOrbLevelType(0, currentFireOrb);
            }else{
                game_p->ui->setOrbLevelType(1, currentIceOrb);
            }
        }
        game_p->ui->setMana(mana);
        for(int i = 1; i < 3; i++){
            game_p->ui->setAmountOfOrbLevelType(0, i, orbInv[i]);
            game_p->ui->setAmountOfOrbLevelType(1, i, orbInv[i+3]);
        }

        game_p->ui->setHealthPoints(healthPoints);
        game_p->ui->setRuneKey(runeKey-1);

  //  std::cout << "end dir.x = " << dir.x << " dir.y = " << dir.y << " castorb = " << currentOrb << "\n";

}

void Player::hit(int health, Direction d){
    if(alive && playerStance != Stance::HIT){
        healthPoints -= health;
        if(d == Direction::LEFT){
            dir.x = -2;
        }else{
            dir.x = 2;
        }
        dir.y = -4;
        y -= 2;
        setStance(Stance::HIT);
        SDL_SetTextureColorMod(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), 255, 32, 32);

        //because no animations are played is it safe to set time to current time
        //during hit state time is used to count how much player will be colored red
        //in our case 100ms
        time = SDL_GetTicks();

        //timer for amount of time in hit mode (unable to move)
        hitTime = time;
    }

    //std::cout << "dir.x = " << dir.x << " dir.y = " << dir.y << "\n";
}

void Player::render(Camera* camera)
{
    //SET POS FOR DRAWING
    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;

    if(playerDirection == Direction::LEFT){
        SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
        camera->RenderCopyEx(Game::SPRITESHEET.at("PlayerBlue").getSheetTexture(), Game::SPRITESHEET.at("PlayerBlue").getSpriteRectAt(currentSprite), &renderRect, 0, 0, flip);
    }else{
        camera->RenderCopy(Game::SPRITESHEET.at("PlayerBlue").getSheetTexture(), Game::SPRITESHEET.at("PlayerBlue").getSpriteRectAt(currentSprite), &renderRect);
    }

    if(sparkleAni->getCurrentSprite() != -1) sparkleAni->render(camera);

    //DEBUG
    //camera->RenderDrawRect(&staffCollision);

}

void Player::land(){
    if(playerStance != Stance::DEAD){
        dir.Set(dir.x,0);
        if(playerStance == Stance::FALLING) setStance(Stance::STANDING);
    }
}


void Player::setDirection(Direction dir){
    playerDirection = dir;
}

void Player::setStance(Stance stance){
    switch(stance){
    case Stance::STANDING:
        currentSprite = 0;
        break;

    case Stance::RUNNING:
        currentSprite = 6;
        break;

    case Stance::DEAD:
        currentSprite = 4;
        break;

    case Stance::FALLING:
        currentSprite = 12;
        break;

    case Stance::ATTACK:
        currentSprite = 18;
        break;

    case Stance::HIT:
        currentSprite = 18;
        break;
    }

    playerStance = stance;
}


void Player::attack(){
    if(playerStance != Stance::ATTACK){
        if(currentOrb == -1) {
            setStance(Stance::ATTACK);
        }else{
            if(currentOrb == 0 || currentOrb == 3){
                if(castOrb(currentOrb)) setStance(Stance::ATTACK);
            }else if(orbInv[currentOrb]-1 >= 0){
                if(castOrb(currentOrb)){
                    orbInv[currentOrb]--;
                    setStance(Stance::ATTACK);
                }
            }
        }
    }
}




void Player::setCurrentOrb(int orb){
    currentOrb = orb;
}

int Player::getCurrentOrb(){
    return currentOrb;
}

void Player::setHealthPoints(int health){
    if(health>6) health = 6;
    healthPoints = health;
}

int Player::getHealthPoints(){
    return healthPoints;
}

void Player::healAnimationStart(){
    sparkleAni->setSpriteSheetKey("HealAnimation");
    sparkleAni->startAnimation();
}

void Player::manaAnimationStart(){
    sparkleAni->setSpriteSheetKey("ManaAnimation");
    sparkleAni->startAnimation();
}

void Player::runeAnimationStart(){
    sparkleAni->setSpriteSheetKey("RuneAnimation");
    sparkleAni->startAnimation();
}




bool Player::castOrb(int orb){
    if(mana-orbMana[orb] < 0) return false;

    mana -= orbMana[orb];
    Orb* castingOrb = new Orb(game_p, orbs[orb], x,y,orbSize[orb],orbSize[orb]);
    SDL_Rect* box = getCollisionRect();
    castingOrb->setY((box->y + box->h/2) - castingOrb->getW()/2);
    if(orb <=2){
        if(playerDirection == Direction::LEFT) castingOrb->setX(box->x -12 - castingOrb->getW()/2);
        else castingOrb->setX(box->x + box->w + 12 - (castingOrb->getW()/2));
    }else{
        if(playerDirection == Direction::LEFT) castingOrb->setX(box->x - 24 - castingOrb->getW()/2);
        else castingOrb->setX(box->x + box->w + 12 - (castingOrb->getW()/4));
    }
    castingOrb->shoot(orb, orbPower[orb], orbFrezze[orb], orbSpeed[orb], (1-(int)(playerDirection)) * 180.0);

    game_p->entities.push_back(castingOrb);

    return true;

}


