#define NOMINMAX

#include "RuneKey.h"

#include <Entity.h>
#include <Player.h>

#include <algorithm>

#include "../Engine/Camera.h"
#include "../Engine/Game.h"
#include "../Engine/Level.h"
#include "../Engine/SpriteSheet.h"
#include "Block.h"

RuneKey::RuneKey(Game* g, float x, float y, float w, float h) : Entity(g, "RuneKey", x, y, w, h) {
    currentSprite = 0;

    dir.x = 0;
    dir.y = -1;
    accel.x = 0;
    accel.y = 0.2;

    timeDropped = SDL_GetTicks();
}

RuneKey::~RuneKey() {
    // dtor
}

void RuneKey::update() {
    collisionRect.x = x;
    collisionRect.y = y;

    dir.x += accel.x;
    dir.y += accel.y;
    x += dir.x;
    y += dir.y;

    checkCollision(game_p->level);
    checkPlayerCollision();
}

void RuneKey::render(Camera* camera) {
    renderRect.x = x;
    renderRect.y = y;

    // DEGUB
    // camera->RenderDrawRect(&collisionRect);

    camera->RenderCopy(
        Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), Game::SPRITESHEET.at(spriteSheetKey).getSpriteRectAt(currentSprite), &renderRect);
}

void RuneKey::checkCollision(Level* level) {
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
                } else {
                    x -= xDepth - 1;
                }
            } else {
                if(distVec.y > 0) {
                    y += yDepth - 1;
                    dir.y = 0.1;
                } else {
                    y -= yDepth - 1;
                    dir.y = 0;
                }
            }
        }
    }
}

void RuneKey::checkPlayerCollision() {
    if(SDL_GetTicks() - timeDropped > 500) {
        if(Game::checkBoxCollision(game_p->p1->getCollisionRect(), getCollisionRect())) {
            Player* p = game_p->p1;
            p->runeAnimationStart();
            p->setRuneKey(p->getRuneKey() + 1);
            setAlive(0);
        }
    }
}
