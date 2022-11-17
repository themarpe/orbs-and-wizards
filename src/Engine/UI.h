#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <Image.h>
#include <GameEngine.h>

class SpriteRectangle;
class Game;

class UI
{
    public:
        UI();
        UI(SDL_Renderer*);
        virtual ~UI();

        void render(SDL_Renderer*);

        void setHealthPoints(int);
        int getHealthPoints();

        void setMana(int);
        int getMana();

        void setWeapon(int);
        int getWeapon();
        void setOrbLevelType(int, int);

        void setAmountOfOrbLevelType(int,int, int);

        void setRuneKey(int a){
            if(a>2) runeKeyIndex = 2;
            else runeKeyIndex = a;
        }

        int getRuneKey(){
            return runeKeyIndex;
        }

    protected:
        int runeKeyIndex;

        SDL_Renderer* renderer;
        TTF_Font* returnFont;

        SDL_Color whiteColor;

        int manaAmount;

        SDL_Rect hearts[3];

        SDL_Texture* fireorb[3];
        SDL_Texture* iceorb[3];
        SDL_Texture* staff;
        SDL_Rect df[3];
        SDL_Rect di[3];
        SDL_Rect ds;

        SDL_Texture* manaText;
        SDL_Rect manaTextRect;
        SDL_Rect manaBar;

        SDL_Texture* manaBox;
        SDL_Rect manaBoxRect;

        SDL_Texture* heartBox;
        SDL_Rect heartBoxRect;

        SDL_Texture* frame;
        SDL_Rect frameDist[3];

        SDL_Texture* num1;
        SDL_Texture* num2;
        SDL_Texture* num3;

        SDL_Rect num1Rect;
        SDL_Rect num2Rect;
        SDL_Rect num3Rect;

        SDL_Rect inv1Rect;
        SDL_Rect inv2Rect;
        SDL_Rect inv3Rect;

        SDL_Texture* runeKey[3];
        SDL_Rect runeKeyRect;


        int orbInvLevel[2][3];
        int orbLevel[2];
        int weapon;


        Uint32 ctr = 0;

        int healthPoints;

    private:
};

#endif // UI_H
