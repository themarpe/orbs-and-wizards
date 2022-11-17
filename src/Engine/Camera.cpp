#include "Camera.h"


Camera::Camera(SDL_Renderer* r, float x, float y, float w, float h){
    renderer = r;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    newDest = new SDL_Rect;
}

Camera::~Camera()
{
    delete newDest;
}

int Camera::RenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect){
    SDL_Rect* tmp = NULL;
    if(dstrect){
        newDest->x = (dstrect->x)-x;
        newDest->y = (dstrect->y)-y;
        newDest->w = dstrect->w;
        newDest->h = dstrect->h;
        tmp = newDest;
    }
    return SDL_RenderCopy(renderer, texture, srcrect, tmp);
}


int Camera::RenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, const SDL_Point* center, const SDL_RendererFlip flip){
    SDL_Rect* tmp = NULL;
    if(dstrect){
        newDest->x = (dstrect->x)-x;
        newDest->y = (dstrect->y)-y;
        newDest->w = dstrect->w;
        newDest->h = dstrect->h;
        tmp = newDest;
    }
    return SDL_RenderCopyEx(renderer, texture, srcrect, tmp, angle, center, flip);
}


int Camera::RenderDrawRect(const SDL_Rect* rect){
    SDL_Rect* tmp = NULL;
    if(rect){
        newDest->x = (rect->x)-x;
        newDest->y = (rect->y)-y;
        newDest->w = rect->w;
        newDest->h = rect->h;
        tmp = newDest;
    }
    return SDL_RenderDrawRect(renderer, tmp);
}

int Camera::RenderDrawLine(int x1, int y1, int x2, int y2){
    return SDL_RenderDrawLine(renderer, x1-x, y1-y, x2-x, y2-y);
}


int Camera::getX(){
    return x;
}
int Camera::getY(){
    return y;
}

void Camera::setX(int a){
    x = a;
}
void Camera::setY(int a){
    y = a;
}

int Camera::getW(){
    return w;
}
int Camera::getH(){
    return h;
}
void Camera::setW(int a){
    w = a;
}
void Camera::setH(int a){
    h = a;
}
