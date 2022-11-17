#include "LevelCreator.h"

using namespace rapidjson;

LevelCreator::LevelCreator(SDL_Renderer* render, int levelWidth, int levelHeight, int tileSize, std::string bgPath, std::string tsPath) {
    renderer = render;

    this->tileSize = tileSize;
    this->levelWidth = levelWidth;
    this->levelHeight = levelHeight;
    backgroundPath = bgPath;
    tilesetPath = tsPath;

    // Create dynamic array of integers
    levelData = new int*[levelHeight];
    for(int i = 0; i < levelHeight; i++) {
        levelData[i] = new int[levelWidth];
    }

    for(int i = 0; i < levelHeight; i++)
        for(int j = 0; j < levelWidth; j++) levelData[i][j] = -1;

    SDL_Rect r;
    std::vector<SDL_Rect> v;
    tileset = SpriteSheet(tilesetPath.c_str(), std::vector<SDL_Rect>(), renderer);
    SDL_QueryTexture(tileset.getSheetTexture(), NULL, NULL, &tileSetW, &tileSetH);
    for(int i = 0; i < tileSetH / tileSize; i++) {
        for(int j = 0; j < tileSetW / tileSize; j++) {
            r.x = j * tileSize;
            r.y = i * tileSize;
            r.w = tileSize;
            r.h = tileSize;
            v.push_back(r);
        }
    }
    tileset.setSprites(v);
    v.clear();

    background = SpriteSheet(backgroundPath.c_str(), std::vector<SDL_Rect>(), renderer);
    int bgW, bgH;
    SDL_QueryTexture(background.getSheetTexture(), NULL, NULL, &bgW, &bgH);
    r.x = 0;
    r.y = 0;
    r.w = bgW;
    r.h = bgH;
    v.push_back(r);
    background.setSprites(v);
    v.clear();
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = ((levelHeight * tileSize) / (float)bgH) * bgW;
    backgroundRect.h = levelHeight * tileSize;

    printf("bg rect = %d h = %d , bgW = %d , bgH = %d", backgroundRect.w, backgroundRect.h, bgW, bgH);

    entityMap[Entities::Door] = new Door(NULL, 0, 0, 64, 64);
    entityMap[Entities::FireRune2] = new Rune(NULL, 0, 0, 32, 32, RuneType::FIRE2);
    entityMap[Entities::FireRune3] = new Rune(NULL, 0, 0, 32, 32, RuneType::FIRE3);
    entityMap[Entities::IceRune2] = new Rune(NULL, 0, 0, 32, 32, RuneType::ICE2);
    entityMap[Entities::IceRune3] = new Rune(NULL, 0, 0, 32, 32, RuneType::ICE3);
    entityMap[Entities::ManaPotion] = new Potion(NULL, 0, 0, 32, 32, PotionType::MANA);
    entityMap[Entities::HealthPotion] = new Potion(NULL, 0, 0, 32, 32, PotionType::HEALTH);
    entityMap[Entities::Player] = new Player(NULL, "PlayerBlue", 0, 0, 64, 64, 6);
    entityMap[Entities::RuneKey] = new RuneKey(NULL, 0, 0, 32, 32);
    entityMap[Entities::Skeleton] = new Skeleton(NULL, "SkeletonWhite", 0, 0, 32, 64);
}

void LevelCreator::exportLevel(std::string lvlName, std::string path) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();
    writer.String("levelName");
    writer.String(lvlName.c_str());
    writer.String("levelWidth");
    writer.Uint(levelWidth);
    writer.String("levelHeight");
    writer.Uint(levelHeight);
    writer.String("tileSize");
    writer.Uint(tileSize);
    writer.String("tileSetPath");
    writer.String(tilesetPath.c_str());
    writer.String("backgroundPath");
    writer.String(backgroundPath.c_str());
    writer.String("levelData");
    writer.StartArray();
    for(unsigned i = 0; i < levelHeight; i++) {
        writer.StartArray();
        for(int j = 0; j < levelWidth; j++) {
            writer.Int(levelData[i][j]);
        }
        writer.EndArray();
    }
    writer.EndArray();
    writer.EndObject();

    std::ofstream outputFile;
    outputFile.open(path);
    outputFile << s.GetString();
    outputFile.close();
}

LevelCreator::~LevelCreator() {
    for(auto e : entityMap) {
        delete e.second;
    }
    background.destroy();
    tileset.destroy();
}

void LevelCreator::update() {}

void LevelCreator::render(Camera* camera) {
    SDL_SetRenderDrawColor(camera->renderer, 0, 0, 0, 255);
    for(int j = 0; j < levelWidth * tileSize; j = j + backgroundRect.w) {
        backgroundRect.x = j;
        camera->RenderCopy(background.getSheetTexture(), background.getSpriteRectAt(0), &backgroundRect);
        SDL_ClearError();
    }

    for(int i = 0; i < levelHeight; i++) {
        for(int j = 0; j < levelWidth; j++) {
            camera->RenderDrawLine(j * tileSize, i * tileSize, j * tileSize, (i + 1) * tileSize);
            camera->RenderDrawLine(j * tileSize, i * tileSize, (j + 1) * tileSize, i * tileSize);
            if(levelData[i][j] >= 0) {
                SDL_Rect dst;
                dst.x = j * tileSize;
                dst.y = i * tileSize;
                dst.w = tileSize;
                dst.h = tileSize;
                camera->RenderCopy(tileset.getSheetTexture(), tileset.getSpriteRectAt(levelData[i][j]), &dst);
            } else if(levelData[i][j] < -1) {
                Entity* tmp = entityMap.at((Entities)levelData[i][j]);
                tmp->setX(j * tileSize);
                tmp->setY(i * tileSize);
                tmp->render(camera);
            }
        }
    }
}
