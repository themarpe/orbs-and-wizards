#pragma once
#include <Camera.h>
#include <Door.h>
#include <Entity.h>
#include <Enums.h>
#include <Player.h>
#include <Potion.h>
#include <Rune.h>
#include <RuneKey.h>
#include <Skeleton.h>
#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <fstream>
#include <map>
#include <streambuf>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class LevelCreator {
   public:
    LevelCreator(){};
    LevelCreator(SDL_Renderer* render, int levelWidth, int levelHeight, int tileSize, std::string bgPath, std::string tsPath);
    virtual ~LevelCreator();

    void update();
    void render(Camera* camera);

    void exportLevel(std::string, std::string);

    void setLevelData(int data, int x, int y) {
        if(x >= 0 && x < levelWidth && y >= 0 && y < levelHeight) {
            levelData[y][x] = data;
        }
    }

    int getLevelData(int x, int y) {
        if(x >= 0 && x < levelWidth && y >= 0 && y < levelHeight) {
            return levelData[y][x];
        }
        return -1;
    }

    int getTileSize() {
        return tileSize;
    }

    int getLevelWidth() {
        return levelWidth;
    }

    int getLevelHeight() {
        return levelHeight;
    }

    int getTilesetWidth() {
        return tileSetW;
    }

    int getTilesetHeight() {
        return tileSetH;
    }

    SpriteSheet tileset;

   protected:
    std::map<Entities, Entity*> entityMap;

    std::string backgrounds[6];

    int tileSize;
    int levelWidth, levelHeight;
    std::string backgroundPath;
    std::string tilesetPath;

    int** levelData;

    int tileSetW, tileSetH;

    SpriteSheet background;
    SDL_Rect backgroundRect;

    SDL_Renderer* renderer;

   private:
};
