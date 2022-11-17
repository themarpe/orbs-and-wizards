#pragma once

#include <SDL.h>
#include <State.h>

class LevelCreatorSelector : public State{

public:
    void init(GameEngine* gameEngine);
    void destroy();
    void pause();
    void resume();
    void handleInput(GameEngine* game);
    void update(GameEngine* game);
    void render(GameEngine* game);

    static LevelCreatorSelector* getInstance(){
        static LevelCreatorSelector instance;
        return &instance;
    }

protected:
private:
    SDL_Renderer* renderer;


    TTF_Font* returnFont;
    SDL_Color whiteColor;

    SDL_Texture* createText;
    SDL_Rect createRect;
    SDL_Rect createSelectedRect;
    bool createSelected = false;

    int levelSizeSelected = 30;
    SDL_Rect sizeMinusRect;
    SDL_Rect sizePlusRect;
    SDL_Rect sizeRect;

    SDL_Texture* pointer;
    SDL_Rect pointerLeftRect;
    SDL_Rect pointerRightRect;

    int currentBackground = 0;

    SDL_Texture* backgrounds[6];
    std::string backgroundParentPath = "Assets/Backgrounds/";
    std::string backgroundFiles[6] = {
        "back_cave.png",
        "back_cave_inverted.png",
        "bubble_background_blue.png",
        "bubble_background_green.png",
        "bubble_background_pink.png",
        "bubble_background_red.png"
    };

    LevelCreatorSelector() {}
    LevelCreatorSelector(LevelCreatorSelector const&);
    void operator=(LevelCreatorSelector const&);

};
