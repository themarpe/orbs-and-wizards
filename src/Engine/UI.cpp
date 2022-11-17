#include "UI.h"

#include <Game.h>
#include <SpriteSheet.h>

UI::UI() {
    healthPoints = 0;
}

UI::UI(SDL_Renderer* renderer) {
    weapon = -1;
    orbLevel[0] = 0;
    orbLevel[1] = 0;

    orbInvLevel[0][1] = 2;
    orbInvLevel[0][2] = 1;

    orbInvLevel[1][1] = 2;
    orbInvLevel[1][2] = 1;

    healthPoints = 6;

    this->renderer = renderer;

    SDL_Rect r;
    std::vector<SDL_Rect> v;

    r.x = 0;
    r.y = 0;
    r.w = 27;
    r.h = 30;
    v.push_back(r);

    r.x = 0;
    r.y = 0;
    r.w = 13;
    r.h = 30;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("Heart", SpriteSheet("res/HUD/heart.png", v, renderer)));
    v.clear();

    r.x = 0;
    r.y = 0;
    r.w = 27;
    r.h = 30;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("HeartDark", SpriteSheet("res/HUD/heart.png", v, renderer)));
    v.clear();
    SDL_SetTextureColorMod(Game::SPRITESHEET["HeartDark"].getSheetTexture(), 40, 40, 40);

    r.x = 0;
    r.y = 0;
    r.w = 800;
    r.h = 55;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("UI_Bar", SpriteSheet("res/HUD/ui_bar.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 3; i++) {
        hearts[i].w = 27;
        hearts[i].h = 30;
        hearts[i].y = 602;
    }
    hearts[0].x = 680;
    hearts[1].x = 720;
    hearts[2].x = 760;

    for(int i = 0; i < 3; i++) {
        std::string path = "res/HUD/fireorb";
        path += (char)((i + 1) + 48);
        path += ".png";
        fireorb[i] = Image::loadImage(renderer, path);
        df[i].x = 170;
        df[i].y = 593;
        df[i].w = 45;
        df[i].h = 45;
        std::cout << path << " end of path \n";
    }

    for(int i = 0; i < 3; i++) {
        std::string path = "res/HUD/iceorb";
        path += (char)((i + 1) + 48);
        path += ".png";
        std::cout << path << " end of path \n";
        iceorb[i] = Image::loadImage(renderer, path);
        std::cout << "sdl = " << IMG_GetError() << " pointer = " << iceorb[i] << "\n";
        di[i].x = 220;
        di[i].y = 593;
        di[i].w = 45;
        di[i].h = 45;
    }
    staff = Image::loadImage(renderer, "res/HUD/staff.png");
    ds.x = 120;
    ds.y = 593;
    ds.w = 45;
    ds.h = 45;

    frame = Image::loadImage(renderer, "res/HUD/frame.png");
    frameDist[0] = ds;
    frameDist[1] = df[0];
    frameDist[2] = di[0];

    std::cout << " make it to the fonts \n";

    returnFont = TTF_OpenFont("res/Fonts/return.ttf", 24);
    whiteColor.r = 255;
    whiteColor.g = 255;
    whiteColor.b = 255;
    SDL_Surface* tmp = TTF_RenderText_Solid(returnFont, "MANA", whiteColor);
    manaText = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);

    tmp = TTF_RenderText_Solid(returnFont, "1", whiteColor);
    num1 = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(num1, NULL, NULL, &num1Rect.w, &num1Rect.h);
    num1Rect.x = frameDist[0].x + 5;
    num1Rect.y = frameDist[0].y + 2;
    inv1Rect.x = num1Rect.x + 20;
    inv1Rect.y = num1Rect.y + 20;

    tmp = TTF_RenderText_Solid(returnFont, "2", whiteColor);
    num2 = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(num2, NULL, NULL, &num2Rect.w, &num2Rect.h);
    num2Rect.x = frameDist[1].x + 5;
    num2Rect.y = frameDist[1].y + 2;
    inv2Rect.x = num2Rect.x + 25;
    inv2Rect.y = num2Rect.y + 22;

    tmp = TTF_RenderText_Solid(returnFont, "3", whiteColor);
    num3 = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_QueryTexture(num3, NULL, NULL, &num3Rect.w, &num3Rect.h);
    SDL_FreeSurface(tmp);
    num3Rect.x = frameDist[2].x + 5;
    num3Rect.y = frameDist[2].y + 2;
    inv3Rect.x = num3Rect.x + 25;
    inv3Rect.y = num3Rect.y + 22;

    int manaX = -270;
    int manaY = 8;

    manaTextRect.x = 283 + manaX;
    manaTextRect.y = 592 + manaY;
    SDL_QueryTexture(manaText, NULL, NULL, &manaTextRect.w, &manaTextRect.h);

    manaBar.x = 280 + manaX;
    manaBar.y = 608 + manaY;
    manaBar.w = 100;
    manaBar.h = 10;

    manaBox = Image::loadImage(renderer, "res/HUD/manaBox.png");
    manaBoxRect.x = 275 + manaX;
    manaBoxRect.y = 605 + manaY;
    manaBoxRect.w = 110;
    manaBoxRect.h = 16;

    heartBox = Image::loadImage(renderer, "res/HUD/heartBox.png");
    heartBoxRect.x = 667;
    heartBoxRect.y = 591;
    heartBoxRect.w = 133;
    heartBoxRect.h = 55;

    runeKey[0] = Image::loadImage(renderer, "res/HUD/runes-blue-1.png");
    runeKey[1] = Image::loadImage(renderer, "res/HUD/runes-blue-2.png");
    runeKey[2] = Image::loadImage(renderer, "res/HUD/runes-blue-3.png");
    runeKeyRect.x = 400;
    runeKeyRect.y = 593;
    runeKeyRect.w = 45;
    runeKeyRect.h = 45;

    manaAmount = 0;

    runeKeyIndex = -1;

    std::cout << " make it past! the fonts \n";
}

void UI::setWeapon(int ot) {
    weapon = ot;
}

int UI::getWeapon() {
    return weapon;
}

void UI::setOrbLevelType(int ot, int ol) {
    orbLevel[ot] = ol;
    weapon = ot;
}

void UI::setMana(int m) {
    manaAmount = m;
}

int UI::getMana() {
    return manaAmount;
}

void UI::setAmountOfOrbLevelType(int ot, int ol, int amount) {
    orbInvLevel[ot][ol] = amount;
}

void UI::render(SDL_Renderer* renderer) {
    int index = (ctr++ / 120) % 3;

    SDL_Rect t;
    t.x = 0;
    t.y = 585;
    t.w = 800;
    t.h = 55;
    SDL_RenderCopy(renderer, Game::SPRITESHEET["UI_Bar"].getSheetTexture(), NULL, &t);

    SDL_RenderCopy(renderer, heartBox, NULL, &heartBoxRect);

    for(int i = 0; i < 3; i++) {
        SDL_RenderCopy(renderer, Game::SPRITESHEET["HeartDark"].getSheetTexture(), NULL, &hearts[i]);
    }
    int i = 0;
    for(i = 0; i < (int)((healthPoints / 2)); i++) {
        SDL_RenderCopy(renderer, Game::SPRITESHEET["Heart"].getSheetTexture(), NULL, &hearts[i]);
    }
    if(healthPoints % 2 == 1) {
        SDL_Rect r;
        r.x = hearts[i].x;
        r.y = hearts[i].y;
        r.h = hearts[i].h;
        r.w = 13;
        SDL_RenderCopy(renderer, Game::SPRITESHEET["Heart"].getSheetTexture(), Game::SPRITESHEET["Heart"].getSpriteRectAt(1), &r);
    }

    SDL_RenderCopy(renderer, staff, NULL, &ds);

    if(manaAmount < 10) {
        SDL_SetTextureColorMod(fireorb[0], 40, 40, 40);
        SDL_SetTextureColorMod(iceorb[0], 40, 40, 40);
    }
    if(manaAmount < 20) {
        SDL_SetTextureColorMod(fireorb[1], 40, 40, 40);
        SDL_SetTextureColorMod(iceorb[1], 40, 40, 40);
    }
    if(manaAmount < 50) {
        SDL_SetTextureColorMod(fireorb[2], 40, 40, 40);
        SDL_SetTextureColorMod(iceorb[2], 40, 40, 40);
    }
    for(int i = 1; i < 3; i++) {
        if(orbInvLevel[0][i] == 0) SDL_SetTextureColorMod(fireorb[i], 40, 40, 40);
        if(orbInvLevel[1][i] == 0) SDL_SetTextureColorMod(iceorb[i], 40, 40, 40);
    }

    SDL_RenderCopy(renderer, fireorb[orbLevel[0]], NULL, &df[0]);
    SDL_RenderCopy(renderer, iceorb[orbLevel[1]], NULL, &di[0]);
    for(int i = 0; i < 3; i++) {
        SDL_SetTextureColorMod(fireorb[i], 255, 255, 255);
        SDL_SetTextureColorMod(iceorb[i], 255, 255, 255);
    }

    SDL_RenderCopy(renderer, frame, NULL, &frameDist[weapon + 1]);

    std::string tmpStr;
    int tmpInv;
    SDL_Surface* tmp;
    SDL_Texture* txt;
    if(orbLevel[0] != 0) {
        std::string tmpStr = "";
        int tmpInv = orbInvLevel[0][orbLevel[0]];
        do {
            tmpStr.insert(begin(tmpStr), (char)(tmpInv % 10 + 48));
        } while(tmpInv /= 10);
        tmp = TTF_RenderText_Solid(returnFont, tmpStr.c_str(), whiteColor);
        txt = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_FreeSurface(tmp);
        SDL_QueryTexture(txt, NULL, NULL, &inv2Rect.w, &inv2Rect.h);
        SDL_RenderCopy(renderer, txt, NULL, &inv2Rect);
        SDL_DestroyTexture(txt);
    }

    if(orbLevel[1] != 0) {
        tmpStr = "";
        tmpInv = orbInvLevel[1][orbLevel[1]];
        do {
            tmpStr.insert(begin(tmpStr), (char)(tmpInv % 10 + 48));
        } while(tmpInv /= 10);
        tmp = TTF_RenderText_Solid(returnFont, tmpStr.c_str(), whiteColor);
        txt = SDL_CreateTextureFromSurface(renderer, tmp);
        SDL_FreeSurface(tmp);
        SDL_QueryTexture(txt, NULL, NULL, &inv3Rect.w, &inv3Rect.h);
        SDL_RenderCopy(renderer, txt, NULL, &inv3Rect);
        SDL_DestroyTexture(txt);
    }

    SDL_RenderCopy(renderer, manaBox, NULL, &manaBoxRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    manaBar.w = (manaAmount % 101);
    SDL_RenderFillRect(renderer, &manaBar);
    SDL_RenderCopy(renderer, manaText, NULL, &manaTextRect);

    SDL_RenderCopy(renderer, num1, NULL, &num1Rect);
    SDL_RenderCopy(renderer, num2, NULL, &num2Rect);
    SDL_RenderCopy(renderer, num3, NULL, &num3Rect);

    if(runeKeyIndex < 0) {
        SDL_SetTextureColorMod(runeKey[0], 48, 48, 48);
        SDL_RenderCopy(renderer, runeKey[0], NULL, &runeKeyRect);
        SDL_SetTextureColorMod(runeKey[0], 255, 255, 255);
    } else {
        SDL_RenderCopy(renderer, runeKey[runeKeyIndex], NULL, &runeKeyRect);
    }

    // std::cout << "weapon: " <<weapon << " orb0 : " << orbLevel[0] << " orb1 : " << orbLevel[1] <<"\n";
}

UI::~UI() {
    for(int i = 0; i < 3; i++) SDL_DestroyTexture(fireorb[i]);
    for(int i = 0; i < 3; i++) SDL_DestroyTexture(iceorb[i]);
    SDL_DestroyTexture(staff);
    SDL_DestroyTexture(manaText);
    SDL_DestroyTexture(manaBox);
    SDL_DestroyTexture(heartBox);
    SDL_DestroyTexture(frame);
    SDL_DestroyTexture(num1);
    SDL_DestroyTexture(num2);
    SDL_DestroyTexture(num3);
}

void UI::setHealthPoints(int h) {
    healthPoints = h;
}

int UI::getHealthPoints() {
    return healthPoints;
}
