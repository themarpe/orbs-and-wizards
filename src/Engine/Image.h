#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>

#include <string>

class Image {
   public:
    static SDL_Texture* loadImage(SDL_Renderer*, std::string);
};

#endif  // IMAGE_H_INCLUDED
