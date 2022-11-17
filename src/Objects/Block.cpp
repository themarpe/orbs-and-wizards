#include "Block.h"

#include "../Engine/Camera.h"
#include "../Engine/Game.h"
#include "../Engine/SpriteSheet.h"
#include "Entity.h"

Block::Block(Game* game, std::string sheet, float x, float y, float w, float h, bool collidable) : Entity(game, sheet, x, y, w, h) {
    this->collidable = collidable;
}

bool Block::getCollidable() {
    return collidable;
}

void Block::setCollidable(bool col) {
    collidable = col;
}

void Block::update() {}

void Block::render(Camera* camera) {
    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;
    camera->RenderCopy(
        Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), Game::SPRITESHEET.at(spriteSheetKey).getSpriteRectAt(currentSprite), &renderRect);
}
