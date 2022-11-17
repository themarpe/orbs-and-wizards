#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(const char* path, std::vector<SDL_Rect> s, SDL_Renderer* renderer) {
    sheetTexture = IMG_LoadTexture(renderer, path);
    sprites = s;
}

SDL_Texture* SpriteSheet::getSheetTexture() {
    return sheetTexture;
}

void SpriteSheet::setSprites(std::vector<SDL_Rect> s) {
    sprites = s;
}

SDL_Rect* SpriteSheet::getSpriteRectAt(int spriteNum) {
    return &sprites[spriteNum];
}
