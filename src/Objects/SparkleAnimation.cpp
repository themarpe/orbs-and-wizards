
#include <SparkleAnimation.h>

#include "../Engine/Camera.h"
#include "../Engine/Game.h"
#include "../Engine/SpriteSheet.h"

SparkleAnimation::SparkleAnimation() : Entity() {
    currentSprite = -1;
    spriteSheetKey = "HealAnimation";
    time = SDL_GetTicks();
}

SparkleAnimation::~SparkleAnimation() {}

void SparkleAnimation::startAnimation() {
    setCurrentSprite(0);
}

void SparkleAnimation::update() {
    if(SDL_GetTicks() - time > 80 && currentSprite >= 0) {
        if(++currentSprite > 6) {
            currentSprite = -1;
        }
        time = SDL_GetTicks();
    }
}

void SparkleAnimation::render(Camera* camera) {
    if(currentSprite >= 0) {
        renderRect.x = x;
        renderRect.y = y;
        renderRect.w = w;
        renderRect.h = h;
        camera->RenderCopy(
            Game::SPRITESHEET.at(spriteSheetKey).getSheetTexture(), Game::SPRITESHEET.at(spriteSheetKey).getSpriteRectAt(currentSprite), &renderRect);
    }
}
