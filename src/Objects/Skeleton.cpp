#define NOMINMAX

#include "Skeleton.h"

#include <Entity.h>
#include <Player.h>

#include <algorithm>

#include "../Engine/Camera.h"
#include "../Engine/Game.h"
#include "../Engine/Level.h"
#include "../Engine/SpriteSheet.h"
#include "Block.h"

Skeleton::Skeleton(Game* g, std::string key, float x, float y, float w, float h) : HittableEnemy(g, key, x, y, w, h) {
    dir.x = -1;
    dir.y = 0;
    accel.x = 0;
    accel.y = 0.2;
    time = SDL_GetTicks();
    aniSpeed = 125;
    stance = Stance::RUNNING;
    direction = Direction::RIGHT;
    healthPoints = 3;
    speed = 1;
    hitDuration = 0;
    frozenDuration = 0;
}

Skeleton::Skeleton() {}

Skeleton::~Skeleton() {
    // dtor
}

void Skeleton::checkCollision(Level* level) {
    std::vector<Block*> collideNeighbours;
    std::pair<int, int> corners[6];

    corners[0] = std::make_pair(x / level->getTileSize(), y / level->getTileSize());
    corners[1] = std::make_pair(x / level->getTileSize(), (y + h / 2) / level->getTileSize());
    corners[2] = std::make_pair(x / level->getTileSize(), (y + h) / level->getTileSize());
    corners[3] = std::make_pair((x + w) / level->getTileSize(), (y + h) / level->getTileSize());
    corners[4] = std::make_pair(x + w / level->getTileSize(), (y + h / 2) / level->getTileSize());
    corners[5] = std::make_pair((x + w) / level->getTileSize(), (y) / level->getTileSize());

    for(int c = 0; c < 6; c++) {
        int32_t i = corners[c].second, j = corners[c].first;
        if(j >= 0 && j < level->getLevelWidth() / level->getTileSize() && i >= 0 && i < level->getLevelHeight() / level->getTileSize()) {
            if(level->levelData[i][j] != NULL && level->levelData[i][j]->getCollidable()) {
                collideNeighbours.push_back(level->levelData[i][j]);
            } else {
                if(stance != Stance::HIT) {
                    if(c == 2) dir.x = speed;
                    else if(c == 3) dir.x = -speed;
                }
                // std::cout << " c = " << c << "DIRX = " << dir.x << "\n";
            }
        }
    }

    for(int c = 0; c < collideNeighbours.size(); c++) {
        Vector2D tilePos(collideNeighbours[c]->getX() + collideNeighbours[c]->getW() / 2.0, collideNeighbours[c]->getY() + collideNeighbours[c]->getH() / 2.0);
        const float MAX_DISTANCEX = w / 2.0 + collideNeighbours[c]->getW() / 2.0;
        const float MAX_DISTANCEY = h / 2.0 + collideNeighbours[c]->getH() / 2.0;
        Vector2D centerPlayerPos = Vector2D(x + w / 2.0, y + h / 2.0);
        Vector2D distVec = centerPlayerPos - tilePos;

        float xDepth = MAX_DISTANCEX - std::abs(distVec.x);
        float yDepth = MAX_DISTANCEY - std::abs(distVec.y);

        if(xDepth > 0 && yDepth > 0) {
            if(std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
                if(distVec.x > 0) {
                    x += xDepth - 1;
                    dir.x = speed;
                } else {
                    x -= xDepth - 1;
                    dir.x = -speed;
                }
                // if((c == 1 || c == 0) && dir.x != speed) dir.x = speed;
                // if((c == 4 || c == 5) && dir.x != -speed) dir.x = -speed;
            } else {
                if(distVec.y > 0) {
                    y += yDepth + 1;
                    dir.y = 0.1;
                } else {
                    y -= yDepth - 1;
                    land();
                }
            }
        }
    }
}

void Skeleton::checkPlayerCollision() {
    if(Game::checkBoxCollision(game_p->p1->getCollisionRect(), getCollisionRect())) {
        SDL_Rect* pRect = game_p->p1->getCollisionRect();
        float xDepth = (pRect->x + pRect->w / 2) - collisionRect.x - collisionRect.w / 2;
        if(xDepth < 0) {
            game_p->p1->hit(1, Direction::LEFT);
        } else {
            game_p->p1->hit(1, Direction::RIGHT);
        }

        // std::cout << "xDepth = " << xDepth << "\n";
    }
}

void Skeleton::setStance(Stance s, Direction d) {
    stance = s;
    direction = d;
    if(d == Direction::RIGHT) {
        switch(stance) {
            case Stance::STANDING:
            case Stance::RUNNING:
            case Stance::FALLING:
            case Stance::HIT:
            case Stance::FROZEN:
                currentSprite = 0;
                break;
            case Stance::ATTACK:
                currentSprite = 32;
                break;

            case Stance::DEAD:
                frozenDuration = 0;
                currentSprite = 23;
                break;

            case Stance::SPAWN:
                currentSprite = 16;
                break;
        }
    } else if(d == Direction::LEFT) {
        switch(stance) {
            case Stance::STANDING:
            case Stance::RUNNING:
            case Stance::FALLING:
            case Stance::HIT:
            case Stance::FROZEN:
                currentSprite = 8;
                break;
            case Stance::ATTACK:
                currentSprite = 40;
                break;

            case Stance::DEAD:
                frozenDuration = 0;
                currentSprite = 31;
                break;

            case Stance::SPAWN:
                currentSprite = 24;
                break;
        }
    }
}

void Skeleton::land() {
    if(stance != Stance::DEAD) {
        dir.Set(dir.x, 0);
        if(stance != Stance::STANDING) {
            if(stance == Stance::HIT) {
                if(dirHit == Direction::LEFT) {
                    dir.x = -2;
                } else {
                    dir.x = 2;
                }
                setStance(Stance::STANDING, dirHit);
            } else {
                setStance(Stance::STANDING, direction);
            }

            dir.Set((std::abs(dir.x) / dir.x) * speed, 0);
        }
    }
}

void Skeleton::setSpeed(float x, float y) {
    dir.x = x;
    speed = std::abs(x);
    dir.y = y;
}

void Skeleton::hit(int damage) {
    if(stance != Stance::HIT && hitDuration <= 0) {
        healthPoints -= damage;
        hitDuration = 30;
        setStance(Stance::HIT, direction);
    }
}

void Skeleton::hit(int damage, Direction d) {
    if(stance != Stance::HIT && hitDuration <= 0) {
        if(stance != Stance::FROZEN) {
            healthPoints -= damage;
            hitDuration = 5;
            dirHit = (Direction)(1 - ((int)d));
            if(healthPoints > 0) {
                setStance(Stance::HIT, d);
                if(d == Direction::LEFT) dir.x = -2;
                if(d == Direction::RIGHT) dir.x = 2;
                dir.y = -2;
                y -= 2;

                hitDuration = 30;

                // aniSpeed MS to go back to normal color
                time = SDL_GetTicks();
                // because no other animation is playing within that time we can safely do that
            }
        } else if(stance == Stance::FROZEN) {
            if(hitDuration == 0) {
                hit(damage);
            }
        }
    }
}

bool Skeleton::getFrezze() {
    return true;
}

void Skeleton::frezze(int duration) {
    if(stance != Stance::FROZEN) {
        frozenDuration = duration;
        setStance(Stance::FROZEN, direction);
    }
}

int Skeleton::getHealthPoints() {
    return healthPoints;
}

void Skeleton::setHealthPoints(int h) {
    healthPoints = h;
}

bool Skeleton::getHittable() {
    if(stance != Stance::DEAD && stance != Stance::HIT) return true;
    return false;
}

void Skeleton::drop() {
    int r = rand() % 9;
    std::cout << "r = " << r << "\n";
    PotionType pt;
    if(r == 0) {
        pt = PotionType::HEALTH;
        Potion* pot = new Potion(game_p, x, y, 32, 32, pt);
        game_p->addEntity(pot);
    } else if(r >= 1 && r <= 4) {
        // RuneKey* pot = new RuneKey(game_p, x,y, 32,32);
        Potion* pot = new Potion(game_p, x, y, 32, 32, PotionType::MANA);
        game_p->addEntity(pot);
    } else if(r == 5) {
        RuneType rt = RuneType::FIRE2;
        game_p->addEntity(new Rune(game_p, x, y, 32, 32, rt));
    } else if(r == 6) {
        RuneType rt = RuneType::FIRE3;
        game_p->addEntity(new Rune(game_p, x, y, 32, 32, rt));
    } else if(r == 7) {
        RuneType rt = RuneType::ICE2;
        game_p->addEntity(new Rune(game_p, x, y, 32, 32, rt));
    } else if(r == 8) {
        RuneType rt = RuneType::ICE3;
        game_p->addEntity(new Rune(game_p, x, y, 32, 32, rt));
    }
}

void Skeleton::update() {
    // std::cout <<"b4 Stance: = " << (int)stance << " Direction = " << (int) direction << " DIRx = " << dir.x <<" currentSprite = " << currentSprite <<"\n";

    if(healthPoints <= 0 && stance != Stance::HIT) {
        dir.Set(0, 0);
        if(stance != Stance::DEAD) setStance(Stance::DEAD, direction);
    }

    collisionRect.x = x;
    collisionRect.y = y;

    if(stance != Stance::FROZEN && stance != Stance::DEAD) checkCollision(game_p->level);
    if(stance != Stance::DEAD && stance != Stance::FROZEN) checkPlayerCollision();

    if(alive && stance != Stance::FROZEN) {
        x += dir.x;
        y += dir.y;
        dir.x += accel.x;
        dir.y += accel.y;
    }

    if(hitDuration > 0) --hitDuration;
    if(frozenDuration > 0) {
        setStance(Stance::FROZEN, direction);
        --frozenDuration;
    }
    if(stance != Stance::FROZEN) {
        if(dir.x > 0 && direction != Direction::RIGHT) setStance(stance, Direction::RIGHT);
        if(dir.x < 0 && direction != Direction::LEFT) setStance(stance, Direction::LEFT);
    }

    if(SDL_GetTicks() - time > aniSpeed) {
        if(direction == Direction::RIGHT) {
            switch(stance) {
                case Stance::STANDING:
                case Stance::RUNNING:
                case Stance::FALLING:
                    if(++currentSprite >= 8) currentSprite = 0;
                    break;
                case Stance::ATTACK:
                    if(++currentSprite >= 40) currentSprite = 32;
                    break;
                case Stance::DEAD:
                    if(currentSprite == 17) {
                        // RANDOM DROP CODE
                        drop();
                        //////////////////
                    }
                    if(--currentSprite <= 16) {
                        setAlive(0);
                    }
                    break;
                case Stance::SPAWN:
                    if(++currentSprite >= 24) currentSprite = 16;
                    break;

                case Stance::HIT:
                    if(--hitDuration <= 0) setStance(Stance::STANDING, direction);
                    break;

                case Stance::FROZEN:
                    if(--frozenDuration <= 0) setStance(Stance::STANDING, direction);
                    break;
            }
        } else {
            switch(stance) {
                case Stance::STANDING:
                case Stance::RUNNING:
                case Stance::FALLING:
                    if(++currentSprite >= 16) currentSprite = 8;
                    break;
                case Stance::ATTACK:
                    if(++currentSprite >= 48) currentSprite = 40;
                    break;

                case Stance::DEAD:
                    if(currentSprite == 25) {
                        // RANDOM DROP CODE
                        drop();
                        //////////////////
                    }
                    if(--currentSprite <= 24) {
                        setAlive(0);
                    }
                    break;

                case Stance::SPAWN:
                    if(++currentSprite >= 31) currentSprite = 24;
                    break;

                case Stance::HIT:
                    if(--hitDuration <= 0) setStance(Stance::STANDING, direction);
                    break;

                case Stance::FROZEN:
                    if(--frozenDuration <= 0) {
                        setStance(Stance::STANDING, direction);
                        std::cout << "OVER OF FREZZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                    }
                    break;
            }
        }
        time = SDL_GetTicks();
    }

    //  std::cout <<"a4 Stance: = " << (int)stance <<" hitDuration = " << hitDuration <<" frozen duration = " << frozenDuration << "\n";
}

void Skeleton::render(Camera* camera) {
    // DEBUG
    // camera->RenderDrawRect(&collisionRect);

    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;

    // sets color mod
    if(hitDuration > 0) {
        SDL_SetTextureColorMod(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), 255, 32, 32);
    } else if(frozenDuration > 0) {
        SDL_SetTextureColorMod(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), 32, 32, 128);
    }

    camera->RenderCopy(
        Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), Game::SPRITESHEET.at(spriteSheetKey).getSpriteRectAt(currentSprite), &renderRect);
    // Resets color mod
    SDL_SetTextureColorMod(Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), 255, 255, 255);
}
