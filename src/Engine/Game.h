#pragma once //HEADERGUARD

#include <vector>
#include <map>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <SDL_image.h>
#include "Image.h"
#include <Enums.h>
#include <ctime>
#include <fstream>
#include <stack>
#include <queue>

#include <SpriteSheet.h>


class Level;

class Player;
class Orb;
class Block;
class Camera;
class Entity;
class UI;
class GameEngine;

struct ReplayStruct{
    long long frameNumber;
    std::vector<InputCommands> inputCommands;
};


class Game
{
    public:

        enum class TypeOfGame {PLAYABLE, REPLAY};

        Game(GameEngine*);
        Game(GameEngine*, std::string replayFile);
        ~Game();

        UI* ui;
        Camera* camera;
        Level* level;
        Player* p1;
        void run();

        void addEntity(Entity* entity);

        void nextLevel();
        void loadNextLevel();
        void gameOver();

        void saveReplay();

        //STATICS

        static bool checkBoxCollision(SDL_Rect*, SDL_Rect*);

        static std::map<std::string, SpriteSheet> SPRITESHEET;

        std::vector<Entity*> entities;
        SDL_Renderer* renderer;
        SDL_Window* window;

        GameEngine* gameEngine;

        int screenWidth, screenHeight;

        std::string levelPath;

        void handleInput();
        void update();
        void render();

    protected:
    private:

        TypeOfGame typeOfGame;
        std::ifstream replayInputStream;
        std::ofstream replayOutputStream;

        std::queue<ReplayStruct> replayCache;

        long long frameCounter = 0;

        bool nextLevelBool = 0;

        std::stack<Entity*> entitiesToAdd;

        bool running;

};
