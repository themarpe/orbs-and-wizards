#pragma once  // HEADERGUARD

#include <SDL_image.h>
#include <SDL_render.h>

#include <iostream>
#include <vector>

class SpriteSheet {
   public:
    SpriteSheet() {}
    SpriteSheet(const char* path, std::vector<SDL_Rect> s, SDL_Renderer*);
    ~SpriteSheet() {}
    void destroy() {
        if(sheetTexture != nullptr) {
            SDL_DestroyTexture(sheetTexture);
        }
    }
    SDL_Texture* getSheetTexture();
    SDL_Rect* getSpriteRectAt(int);
    void setSprites(std::vector<SDL_Rect>);

   private:
    SDL_Texture* sheetTexture = nullptr;
    std::vector<SDL_Rect> sprites;
};
