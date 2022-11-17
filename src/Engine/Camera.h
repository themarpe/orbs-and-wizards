#pragma once

#include <iostream>
#include "SDL.h"

class Camera
{
    public:
        Camera(){};
        Camera(SDL_Renderer*, float, float, float, float);
        ~Camera();

        int RenderCopy(SDL_Texture*    texture,
                   const SDL_Rect* srcrect,
                   const SDL_Rect* dstrect);

        int RenderCopyEx(SDL_Texture*           texture,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center,
                     const SDL_RendererFlip flip);

        int RenderDrawRect(const SDL_Rect* rect);

        int RenderDrawLine(
                       int           x1,
                       int           y1,
                       int           x2,
                       int           y2);


        int getX();
        int getY();
        void setX(int);
        void setY(int);

        int getW();
        int getH();
        void setW(int);
        void setH(int);

        SDL_Renderer* renderer;

    protected:
        float x,y;
        float w,h;

        SDL_Rect* newDest;

    private:
};

