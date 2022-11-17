#pragma once

#include <SDL.h>
#include <State.h>

#include <States/MainMenuState.h>
#include <Camera.h>
#include <LevelCreator.h>

#include <Vector2D.h>

class LevelCreatorState : public State{

public:
    void init(GameEngine* gameEngine);
    void destroy();
    void pause();
    void resume();
    void handleInput(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static LevelCreatorState* getInstance(){
        static LevelCreatorState instance;
        return &instance;
    }

    std::string backgroundPath;

protected:
private:
    LevelCreatorState() {}
    LevelCreatorState(LevelCreatorState const&);
    void operator=(LevelCreatorState const&);

    Camera* cameraMain;
    Camera* cameraSub;

    int mouseX = 0, mouseY = 0;
    bool activeWindow1 = false;
    bool activeWindow2 = false;

    bool mouseButtonLeft = false;
    bool mouseButtonRight = false;
    bool mouseButtonMiddle = false;

    bool mouseButtonLeftUp = true;
    bool mouseButtonRightUp = true;
    bool mouseButtonMiddleUp = true;


    int screen1Width, screen1Height;
    int screen2Width, screen2Height;


    int win1x, win1y;


    SDL_Window* window2;
    SDL_Renderer* renderer2;
    int win2x, win2y;


    SDL_Rect tilesetrend1Rect;

    SDL_Renderer* renderer;

    SDL_Texture* frameTexture;
    SDL_Rect frameRect;

    SDL_Texture* tileset;
    SDL_Rect tilesetRect;

    int pointerGrabbedInt = 0;

    LevelCreator* levelCreator;

    SDL_Rect redRect;

    SDL_Rect iconsRect[10];
    SDL_Texture* iconsText[10];
    Entities iconsEntities[10];

    bool playerPlaced;
    bool doorPlaced;

    std::string levelNameBuffer = "dank level 123";
    SDL_Rect nameInputRect;
    SDL_Rect nameInputPointer;
    bool nameInputPointerClicked;

    SDL_Texture* levelNameText;
    SDL_Rect levelNameRect;

    SDL_Texture* createText;
    SDL_Rect createRect;

    SDL_Texture* pointer;
    SDL_Rect pointerRect;
    bool createSelected;

    TTF_Font* returnFont48;
    TTF_Font* returnFont24;
    SDL_Color whiteColor;
    SDL_Color blackColor;

    SDL_Texture* levelCreatedScreen;

    uint32_t time;

    bool createBool = false;

    Vector2D dragClicked;
    Vector2D cameraClicked;


};
