#include "Level.h"

#include <Door.h>
#include <GameEngine.h>
#include <Player.h>

#include <cstdio>

#include "../Objects/Block.h"
#include "../Objects/Skeleton.h"
#include "Camera.h"
#include "Game.h"
#include "SpriteSheet.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

Level::Level(Game* game, std::string levelPath) {
    game_p = game;
    // INITS

    FILE* fp = fopen(levelPath.c_str(), "rb");  // non-Windows use "r"
    char readBuffer[65536];

    if(!fp) {
        std::cout << "Invalid file selected\n";
    }

    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    ParseResult result = d.ParseStream(is);
    fclose(fp);

    if(!result) {
        printf("JSON parse error: %s (%zu)", GetParseError_En(result.Code()), result.Offset());
    }

    levelName = d["levelName"].GetString();
    bgName = levelName + "_bg";

    levelWidth = d["levelWidth"].GetInt();
    levelHeight = d["levelHeight"].GetInt();
    tileSize = d["tileSize"].GetInt();
    std::string tileSetPath = d["tileSetPath"].GetString();
    std::string backgroundPath = d["backgroundPath"].GetString();

    SDL_Rect r;
    std::vector<SDL_Rect> v;

    // SAVES TEXTURE OF BACKGROUND
    SDL_ClearError();

    Game::SPRITESHEET[bgName] = SpriteSheet(backgroundPath.c_str(), v, game_p->renderer);

    // Game::SPRITESHEET.insert(std::make_pair<std::string, SpriteSheet>(bgName,  SpriteSheet(backgroundPath.c_str(), v, game_p->renderer)));

    std::cout << "GetError() = " << SDL_GetError() << " background path = " << backgroundPath << "\n";

    SpriteSheet spr(backgroundPath.c_str(), std::vector<SDL_Rect>(), game_p->renderer);
    std::cout << "GetError() = " << SDL_GetError() << " background texture pointer = " << spr.getSheetTexture() << "\n";
    int bgWidth, bgHeight;
    SDL_QueryTexture(Game::SPRITESHEET.at(bgName).getSheetTexture(), NULL, NULL, &bgWidth, &bgHeight);
    std::cout << "bgName spritesheet = " << Game::SPRITESHEET[bgName].getSheetTexture();
    std::cout << "bgWidth = " << bgWidth << " bgHeight = " << bgHeight << "\n";

    // CREATES RECTS OF THE TILES
    r.x = 0;
    r.y = 0;
    r.w = bgWidth;
    r.h = bgHeight;
    v.push_back(r);
    Game::SPRITESHEET.at(bgName).setSprites(v);
    bgRect.x = 0;
    bgRect.y = 0;
    bgRect.w = ((levelHeight * tileSize) / (float)bgHeight) * bgWidth;
    bgRect.h = levelHeight * tileSize;

    // SETS RECTS TO TILESET

    std::cout << "tileSetPath " << tileSetPath << "levelWidth = " << levelWidth << " levelHeight = " << levelHeight << " tileSize = " << tileSize << "\n";

    // SAVES TEXTUER

    Game::SPRITESHEET.insert(std::pair<std::string, SpriteSheet>(levelName, SpriteSheet(tileSetPath.c_str(), v, game_p->renderer)));

    std::cout << "GetError = " << SDL_GetError() << "\n";
    int tileSetWidth, tileSetHeight;
    SDL_QueryTexture(Game::SPRITESHEET.at(levelName).getSheetTexture(), NULL, NULL, &tileSetWidth, &tileSetHeight);

    std::cout << "tileSetWidth =  " << tileSetWidth << "tileSetHeight = " << tileSetHeight << "\n";

    int tileSetTilesX = tileSetWidth / tileSize;
    int tileSetTilesY = tileSetHeight / tileSize;

    // CREATES RECTS OF THE TILES
    v.clear();
    for(int i = 0; i < tileSetTilesY; i++)
        for(int j = 0; j < tileSetTilesX; j++) {
            r.x = j * tileSize;
            r.y = i * tileSize;
            r.w = tileSize;
            r.h = tileSize;
            v.push_back(r);
        };
    Game::SPRITESHEET.at(levelName).setSprites(v);
    v.clear();

    // SETS RECTS TO TILESET

    // DECLARATION AND ALLOCATION
    levelData = new Block**[levelHeight];
    for(int i = 0; i < levelHeight; i++) levelData[i] = new Block*[levelWidth];

    // INIT OF 0
    for(int i = 0; i < levelHeight; i++) {
        for(int j = 0; j < levelWidth; j++) {
            levelData[i][j] = NULL;
        }
    }

    Block* block;
    for(SizeType i = 0; i < d["levelData"].Size(); i++) {
        for(SizeType j = 0; j < d["levelData"][i].Size(); j++) {
            int currentBlock = d["levelData"][i][j].GetInt();
            if(currentBlock >= 0) {
                block = new Block(NULL, levelName, (float)j * tileSize, (float)i * tileSize, (float)tileSize, (float)tileSize, false);
                block->setCurrentSprite(currentBlock);
                block->setCollidable(1);
                levelData[i][j] = block;
            } else {
                // CODE FOR NATIVE OBJECT SPAWNING
                // MOST LIKELY SPAWNER CLASS WITH STATIC METHODS WHICH INCLUDES CASES FOR PLAYERS ORBS ETC ETC
                Entity* entity = NULL;
                switch((Entities)currentBlock) {
                    case Entities::Skeleton:
                        entity = new Skeleton(game_p, "SkeletonWhite", j * tileSize, i * tileSize, 32, 64);
                        break;
                    case Entities::Player:
                        if(game_p->p1 == nullptr) {
                            game_p->p1 = new Player(game_p, "PlayerBlue", j * tileSize, i * tileSize, 64, 64, 6);
                        } else {
                            game_p->p1->setX(j * tileSize);
                            game_p->p1->setY(i * tileSize);
                        }
                        break;

                    case Entities::Door:
                        entity = new Door(game_p, j * tileSize, i * tileSize, 64, 64);
                        break;
                    case Entities::HealthPotion:
                        entity = new Potion(game_p, j * tileSize, i * tileSize, 32, 32, PotionType::HEALTH);
                        break;

                    case Entities::ManaPotion:
                        entity = new Potion(game_p, j * tileSize, i * tileSize, 32, 32, PotionType::MANA);
                        break;

                    case Entities::FireRune2:
                        entity = new Rune(game_p, j * tileSize, i * tileSize, 32, 32, RuneType::FIRE2);
                        break;

                    case Entities::FireRune3:
                        entity = new Rune(game_p, j * tileSize, i * tileSize, 32, 32, RuneType::FIRE3);
                        break;

                    case Entities::IceRune2:
                        entity = new Rune(game_p, j * tileSize, i * tileSize, 32, 32, RuneType::ICE2);
                        break;

                    case Entities::IceRune3:
                        entity = new Rune(game_p, j * tileSize, i * tileSize, 32, 32, RuneType::ICE3);
                        break;

                    case Entities::RuneKey:
                        entity = new RuneKey(game_p, j * tileSize, i * tileSize, 32, 32);
                        break;
                }

                if(entity) game_p->addEntity(entity);
            }
        }
    }
}

Level::~Level() {
    for(auto itr = Game::SPRITESHEET.begin(); itr != Game::SPRITESHEET.end(); ++itr) {
        if(itr->first == bgName) {
            std::cout << "deleting asset: " << itr->first << "\n";
            itr = Game::SPRITESHEET.erase(itr);
            break;
        }
    }
    for(auto itr = Game::SPRITESHEET.begin(); itr != Game::SPRITESHEET.end(); ++itr) {
        if(itr->first == levelName) {
            std::cout << "deleting asset: " << itr->first << "\n";
            itr = Game::SPRITESHEET.erase(itr);
        }
    }

    for(int i = 0; i < levelHeight; i++) {
        for(int j = 0; j < levelWidth; j++) {
            if(levelData[i][j] != NULL) {
                delete levelData[i][j];
                levelData[i][j] = NULL;
            }
        }
    }

    for(int i = 0; i < levelHeight; i++) delete[] levelData[i];
    delete[] levelData;
}

int Level::getLevelWidth() {
    return levelWidth * tileSize;
}

std::string Level::getLevelName() {
    return levelName;
}

int Level::getLevelHeight() {
    return levelHeight * tileSize;
}

int Level::getTileSize() {
    return tileSize;
}

void Level::update() {}

void Level::render(Camera* camera) {
    SDL_Rect* r = Game::SPRITESHEET.at(bgName).getSpriteRectAt(0);

    // std::cout << "rendering level... rectx = " << r->x << " recty " << r->y << " rectw " << r->w << " recth " << r->h << "\n";

    for(int j = 0; j < levelWidth * tileSize; j = j + bgRect.w) {
        bgRect.x = j;
        camera->RenderCopy(Game::SPRITESHEET.at(bgName).getSheetTexture(), Game::SPRITESHEET.at(bgName).getSpriteRectAt(0), &bgRect);
    }

    for(int i = 0; i < levelHeight; i++) {
        for(int j = 0; j < levelWidth; j++) {
            if(levelData[i][j] != NULL) levelData[i][j]->render(camera);
        }
    }
}
