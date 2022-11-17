#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <SDL.h>
#include <string>
#include <SDL_image.h>

class Image
{
public:
    static SDL_Texture* loadImage( SDL_Renderer* , std::string );
};


#endif // IMAGE_H_INCLUDED
