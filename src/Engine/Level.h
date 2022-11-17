#pragma once //HEADERGUARD

#include <iostream>
#include <vector>
#include "SDL.h"

#include <windows.h>
class Camera;
class Game;
class Block;
class SpriteSheet;
class SpriteRectangle;

class Level
{

    public:
        Level(Game*, std::string);
        ~Level();

        void update();
        void render(Camera*);

        //std::vector<std::vector<Block*> > levelData;

        Block*** levelData;

        int getLevelWidth();
        int getLevelHeight();
        int getTileSize();
        std::string getLevelName();

    protected:
    private:
        Game* game_p;
        std::string levelName;
        std::string bgName;

        SDL_Rect bgRect;

        SDL_Texture* bgTexture;

        int tileSize;
        int levelWidth;
        int levelHeight;

};

