#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <stack>
#include <string>
#include <map>
#include <SpriteSheet.h>

#include <ctime>

#include <Game.h>

class State;

class GameEngine{
public:

    static std::string getDateTime(){

        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

          time (&rawtime);
          timeinfo = localtime(&rawtime);

          strftime(buffer,80,"%d-%m-%Y_%I-%M-%S",timeinfo);
          std::string str(buffer);


       return str;

    }


    void setRngSeed(long long seed){
        rngSeed = seed;
        srand(seed);
    }

    long long getRngSeed(){
        return rngSeed;
    }



    enum class LevelType{NORMAL, CUSTOM, REPLAY};

	void init();
	void destroy();

	void changeState(State* state);
	void pushState(State* state);
	void popState();

	void handleInput();
	void update();
	void render();

	bool getRunning() { return running; }
	void quit() { running = false; }

    void setLevelPath(std::string);

    void setReplayPath(std::string);


    std::string getReplayPath();
    std::string getLevelPath();


	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;

	int screenWidth;
	int screenHeight;

    int getCurrentLevel(){
        return currentLevel;
    }

    void setCurrentLevel(int c){
        currentLevel = c;
    }

    int getMaxLevels(){
        return maxLevels;
    }
    void setMaxLevels(int m){
        maxLevels = m;
    }

    void setLevelType(LevelType lvltype){
        levelType = lvltype;
    }

    LevelType getLevelType(){
        return levelType;
    }

	bool running;

	int maxLevels;
	LevelType levelType;

	struct LevelCreatorData{
        std::string backgroundPath;
        std::string tilesetPath;
        int levelWidth;
        int levelHeight;
        int tileSize;
	} levelCreatorData;

private:

    long long rngSeed;

    int currentLevel = 0;

	// the stack of states
	std::stack<State*> states;


	std::string levelPath;
	std::string replayPath;
};

