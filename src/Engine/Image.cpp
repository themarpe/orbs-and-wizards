#include "Image.h"

SDL_Texture* Image::loadImage(SDL_Renderer* renderer, std::string path) {
    return IMG_LoadTexture(renderer, path.c_str());
}
