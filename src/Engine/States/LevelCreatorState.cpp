#include "LevelCreatorState.h"

#include <GameEngine.h>

void LevelCreatorState::init(GameEngine* gameEngine){
    renderer = gameEngine->renderer;

    window2 = nullptr;
    renderer2 = nullptr;


    tileset = IMG_LoadTexture(renderer, "Assets/Tilesets/tileset0.png");
    SDL_QueryTexture(tileset,NULL,NULL, &tilesetRect.w, &tilesetRect.h);
    tilesetRect.x = 0;
    tilesetRect.y = 0;

    SDL_DisplayMode current;
    int should_be_zero = SDL_GetCurrentDisplayMode(0, &current);
    int xOff = current.w/2 - ((tilesetRect.w+800)/2);

    screen1Width = 800;
    screen1Height = 640;

    SDL_SetWindowPosition(gameEngine->window, xOff, SDL_WINDOWPOS_CENTERED);

    window2 = SDL_CreateWindow("Tile set", xOff+810, SDL_WINDOWPOS_CENTERED, tilesetRect.w,640, SDL_WINDOW_BORDERLESS);
    renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    screen2Width = tilesetRect.w;
    screen2Height = 640;

    SDL_DestroyTexture(tileset);
    tileset = IMG_LoadTexture(renderer2, "Assets/Tilesets/tileset0.png");
    SDL_QueryTexture(tileset,NULL,NULL, &tilesetRect.w, &tilesetRect.h);
    tilesetRect.x = 0;
    tilesetRect.y = 0;

    SDL_SetRenderDrawColor(renderer2, 128,128,128,255);

    cameraMain = new Camera(renderer, 0,0, 800,640);
    cameraSub = new Camera(renderer2, 0,0, tilesetRect.w,tilesetRect.h);

    levelCreator = new LevelCreator(renderer,
        gameEngine->levelCreatorData.levelWidth, gameEngine->levelCreatorData.levelHeight,gameEngine->levelCreatorData.tileSize,
        gameEngine->levelCreatorData.backgroundPath, gameEngine->levelCreatorData.tilesetPath
    );

    frameTexture = IMG_LoadTexture(renderer2, "res/HUD/frame-green.png");
    frameRect.w = levelCreator->getTileSize();
    frameRect.h = levelCreator->getTileSize();

    redRect.w = levelCreator->getTileSize();
    redRect.h = levelCreator->getTileSize();


    for(int i = 0; i<10; i++){
        iconsRect[i].w = 32;
        iconsRect[i].h = 32;
    }
    for(int i = 0; i<4; i++){
        iconsRect[i].x = 32*i;
        iconsRect[i].y = tilesetRect.h;
    }
    for(int i = 4; i<8; i++){
        iconsRect[i].x = 32*(i-4);
        iconsRect[i].y = tilesetRect.h+32;
    }
    iconsRect[8].x = 0;
    iconsRect[8].y = tilesetRect.h+64;
    iconsRect[9].x = 32;
    iconsRect[9].y = tilesetRect.h+64;


    iconsText[0] = IMG_LoadTexture(renderer2, "res/Icons/player.png");
    iconsText[1] = IMG_LoadTexture(renderer2, "res/Icons/doorEnd.png");
    iconsText[2] = IMG_LoadTexture(renderer2, "res/Icons/runekey.png");
    iconsText[3] = IMG_LoadTexture(renderer2, "res/Icons/skeleton.png");

    iconsText[4] = IMG_LoadTexture(renderer2, "res/Icons/fireRune2.png");
    iconsText[5] = IMG_LoadTexture(renderer2, "res/Icons/fireRune3.png");
    iconsText[6] = IMG_LoadTexture(renderer2, "res/Icons/iceRune2.png");
    iconsText[7] = IMG_LoadTexture(renderer2, "res/Icons/iceRune3.png");

    iconsText[8] = IMG_LoadTexture(renderer2, "res/Icons/healthPotion.png");
    iconsText[9] = IMG_LoadTexture(renderer2, "res/Icons/manaPotion.png");


    iconsEntities[0] = Entities::Player;
    iconsEntities[1] = Entities::Door;
    iconsEntities[2] = Entities::RuneKey;
    iconsEntities[3] = Entities::Skeleton;

    iconsEntities[4] = Entities::FireRune2;
    iconsEntities[5] = Entities::FireRune3;
    iconsEntities[6] = Entities::IceRune2;
    iconsEntities[7] = Entities::IceRune3;

    iconsEntities[8] = Entities::HealthPotion;
    iconsEntities[9] = Entities::ManaPotion;


    returnFont48 = TTF_OpenFont("res/Fonts/return.ttf", 48);
    returnFont24 = TTF_OpenFont("res/Fonts/return.ttf", 24);
    whiteColor.a = 255;
    whiteColor.r = 255;
    whiteColor.g = 255;
    whiteColor.b = 255;

    blackColor.a = 255;
    blackColor.r = 0;
    blackColor.g = 0;
    blackColor.b = 0;

    SDL_Surface* tmp = TTF_RenderText_Solid(returnFont48, "Create", whiteColor);
    createText = SDL_CreateTextureFromSurface(renderer2, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(createText, NULL, NULL, &createRect.w, &createRect.h);
    createRect.x = tilesetRect.w - createRect.w - 20;
    createRect.y = 640 - createRect.h - 10;

    pointer = IMG_LoadTexture(renderer2, "res/HUD/pointer.png");
    pointerRect.x = tilesetRect.w - createRect.w - 60;
    pointerRect.y = 640 - createRect.h - 8;
    pointerRect.w = 32;
    pointerRect.h = 32;

    nameInputRect.x = 5;
    nameInputRect.y = tilesetRect.h + 256;
    nameInputRect.w = tilesetRect.w - 48;
    nameInputRect.h = 32;

    nameInputPointer.x = tilesetRect.w - 42;
    nameInputPointer.y = tilesetRect.h + 256;
    nameInputPointer.w = 32;
    nameInputPointer.h = 32;

    tmp = TTF_RenderText_Solid(returnFont48, "Level Name", whiteColor);
    levelNameText = SDL_CreateTextureFromSurface(renderer2, tmp);
    SDL_FreeSurface(tmp);
    SDL_QueryTexture(levelNameText, NULL, NULL, &levelNameRect.w, &levelNameRect.h);
    levelNameRect.x = 10;
    levelNameRect.y = tilesetRect.h + 215;

    levelCreatedScreen = IMG_LoadTexture(renderer, "res/Menus/levelcreatedscreen.png");

    time = SDL_GetTicks();

    createBool = false;
    playerPlaced = false;
    doorPlaced = false;
    nameInputPointerClicked = false;
    createSelected = false;
    pointerGrabbedInt = 0;

    mouseX = 0;
    mouseY = 0;
    activeWindow1 = false;
    activeWindow2 = false;

    mouseButtonLeft = false;
    mouseButtonRight = false;
    mouseButtonMiddle = false;

    mouseButtonLeftUp = true;
    mouseButtonRightUp = true;
    mouseButtonMiddleUp = true;


}

void LevelCreatorState::destroy(){
    SDL_DestroyTexture(tileset);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window2);

    for(int i = 0; i<10; i++){
        SDL_DestroyTexture(iconsText[i]);
    }

    TTF_CloseFont(returnFont48);
    TTF_CloseFont(returnFont24);

    SDL_DestroyTexture(levelNameText);
    SDL_DestroyTexture(createText);
    SDL_DestroyTexture(pointer);
    SDL_DestroyTexture(frameTexture);

    SDL_DestroyTexture(levelCreatedScreen);

    delete levelCreator;

    std::cout << "got destroyed!\n";
}

void LevelCreatorState::pause(){

}

void LevelCreatorState::resume(){

}


void LevelCreatorState::handleInput(GameEngine* gameEngine){

    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.quit.type == SDL_QUIT){
            gameEngine->quit();
        }

        if( e.type == SDL_KEYDOWN ){
            if(e.key.keysym.sym == SDLK_BACKSPACE && levelNameBuffer.length() > 0 && nameInputPointerClicked){
                levelNameBuffer.pop_back();
            }
        }

        if( e.type == SDL_TEXTINPUT && nameInputPointerClicked){
            //Append character
            levelNameBuffer += e.text.text;
        }


        if(e.key.type == SDL_KEYUP){
            switch(e.key.keysym.sym){
            case SDLK_ESCAPE:
                gameEngine->changeState(MainMenuState::getInstance());
                break;

            }
        }
        if(e.motion.type == SDL_MOUSEMOTION){
            if(e.motion.windowID == SDL_GetWindowID(gameEngine->window)){
                activeWindow1 = true;
                activeWindow2 = false;
            }else if(e.motion.windowID == SDL_GetWindowID(window2)){
                activeWindow1 = false;
                activeWindow2 = true;
            }
            mouseX = e.motion.x;
            mouseY = e.motion.y;
        }

        if(e.button.type == SDL_MOUSEBUTTONDOWN){
            switch(e.button.button){
                case SDL_BUTTON_LEFT:
                    mouseButtonLeft = true;
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseButtonRight = true;
                    break;
                case SDL_BUTTON_MIDDLE:
                    mouseButtonMiddle = true;
                    break;
            }

        }

        if(e.button.type == SDL_MOUSEBUTTONUP){
            switch(e.button.button){
                case SDL_BUTTON_LEFT:
                    mouseButtonLeft = false;
                    mouseButtonLeftUp = true;
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseButtonRight = false;
                    mouseButtonRightUp = true;
                    break;
                case SDL_BUTTON_MIDDLE:
                    mouseButtonMiddle = false;
                    mouseButtonMiddleUp = true;
                    break;
            }
        }

    }

}

void LevelCreatorState::update(GameEngine* gameEngine){

    SDL_GetWindowPosition(gameEngine->window, &win1x, &win1y);
    SDL_SetWindowPosition(window2, win1x+810, win1y);

    if(createBool){
        if(SDL_GetTicks() - time > 2000){
            gameEngine->changeState(MainMenuState::getInstance());
        }
    }

    if(activeWindow1){

        if(mouseButtonLeft){
            if((pointerGrabbedInt == (int)Entities::Player && playerPlaced) || (pointerGrabbedInt == (int) Entities::Door && doorPlaced)){
            }else{
                int nx = mouseX+cameraMain->getX();
                int ny = mouseY+cameraMain->getY();
                levelCreator->setLevelData(pointerGrabbedInt, nx/levelCreator->getTileSize(), ny/levelCreator->getTileSize());
            }
            if(pointerGrabbedInt == (int)Entities::Player){
                playerPlaced = true;
            }else if(pointerGrabbedInt == (int) Entities::Door){
                doorPlaced = true;
            }
        }

        if(mouseButtonRight){
            int nx = mouseX+cameraMain->getX();
            int ny = mouseY+cameraMain->getY();
            int tileRemoved = levelCreator->getLevelData(nx/levelCreator->getTileSize(), ny/levelCreator->getTileSize());
            if(tileRemoved == (int) Entities::Player){
                playerPlaced = false;
            }else if(tileRemoved == (int) Entities::Door){
                doorPlaced = false;
            }
            levelCreator->setLevelData(-1, nx/levelCreator->getTileSize(), ny/levelCreator->getTileSize());
        }

        if(mouseButtonMiddle && mouseButtonMiddleUp){
            dragClicked.Set(mouseX, mouseY);
            cameraClicked.Set(cameraMain->getX(), cameraMain->getY());
            mouseButtonMiddleUp = false;
        }

        if(mouseButtonMiddle && !mouseButtonMiddleUp){
            int cx = cameraClicked.x + (dragClicked.x - mouseX);
            int cy = cameraClicked.y + (dragClicked.y - mouseY);
            if(cx >= 0 && cx+screen1Width < levelCreator->getLevelWidth()*levelCreator->getTileSize()){
                 cameraMain->setX(cx);
            }

            if(cy >= 0 && cy+screen1Height < levelCreator->getLevelHeight()*levelCreator->getTileSize()){
                cameraMain->setY(cy);
            }
        }

        int nx = mouseX+cameraMain->getX()%32;
        int ny = mouseY+cameraMain->getY()%32;
        nx/=levelCreator->getTileSize();
        ny/=levelCreator->getTileSize();

        nx*=levelCreator->getTileSize();
        ny*=levelCreator->getTileSize();

        redRect.x = nx - cameraMain->getX()%32;
        redRect.y = ny - cameraMain->getY()%32;

    }else if(activeWindow2){

        if(mouseX > createRect.x && mouseX < createRect.x+createRect.w && mouseY > createRect.y && mouseY < createRect.y+createRect.h){
            createSelected = true;
        }else{
            createSelected = false;
        }

        if(mouseButtonLeft){
            if(createSelected && !createBool && playerPlaced && doorPlaced){
                //CREATE LEVEL FINALLY
                time = SDL_GetTicks();
                std::string t = "CustomLevels/" + levelNameBuffer;
                t += ".lvl";
                levelCreator->exportLevel(levelNameBuffer, t);
                createBool = true;
            }

            if(mouseX > nameInputRect.x && mouseX < nameInputRect.x+nameInputRect.w && mouseY > nameInputRect.y && mouseY < nameInputRect.y+nameInputRect.h){
                nameInputPointerClicked = true;
            }else{
                nameInputPointerClicked = false;
            }

            if(mouseY < tilesetRect.h){
                int nx = mouseX;
                int ny = mouseY;
                pointerGrabbedInt = ny/levelCreator->getTileSize() * levelCreator->getTilesetWidth()/levelCreator->getTileSize() + nx/levelCreator->getTileSize();
                frameRect.x = (nx/levelCreator->getTileSize())*levelCreator->getTileSize();
                frameRect.y = (ny/levelCreator->getTileSize())*levelCreator->getTileSize();
            }else{
                for(int i = 0; i<10; i++){
                    if(mouseX > iconsRect[i].x && mouseX < iconsRect[i].x+iconsRect[i].w && mouseY > iconsRect[i].y && mouseY < iconsRect[i].y+iconsRect[i].h){
                        if(i == 0 && playerPlaced) continue;
                        if(i == 1 && doorPlaced) continue;
                        frameRect.x = iconsRect[i].x;
                        frameRect.y = iconsRect[i].y;
                        pointerGrabbedInt = (int) iconsEntities[i];
                        break;
                    }
                }
            }
        }

    }

   // std::cout << "WINDOW1: mouse x = " << mouseX << " mousey " << mouseY<<" screen1Width = " << screen1Width << " screen1Height = " << screen1Height <<"\n";

}


void LevelCreatorState::render(GameEngine* gameEngine){

    SDL_RenderClear(renderer);
    SDL_RenderClear(renderer2);

    //RENDERER MAIN
    levelCreator->render(cameraMain);
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderDrawRect(renderer, &redRect);
    SDL_SetTextureAlphaMod(levelCreator->tileset.getSheetTexture(), 127);
    if(pointerGrabbedInt>=0) SDL_RenderCopy(renderer, levelCreator->tileset.getSheetTexture(), levelCreator->tileset.getSpriteRectAt(pointerGrabbedInt), &redRect);
    SDL_SetTextureAlphaMod(levelCreator->tileset.getSheetTexture(), 255);

    if(createBool) SDL_RenderCopy(renderer, levelCreatedScreen, NULL, NULL);
    ///////////////////////////////////////////////


    //RENDERER SUB
    SDL_SetRenderDrawColor(renderer2, 128,128,128,255);
    cameraSub->RenderCopy(tileset, NULL, &tilesetRect);
    for(int i = 0; i<10; i++){
        if(i == 0 && playerPlaced){
            SDL_SetTextureColorMod(iconsText[i], 64,64,64);
            cameraSub->RenderCopy(iconsText[i], NULL, &iconsRect[i]);
            SDL_SetTextureColorMod(iconsText[i], 255,255,255);
        }else if(i == 1 && doorPlaced){
            SDL_SetTextureColorMod(iconsText[i], 64,64,64);
            cameraSub->RenderCopy(iconsText[i], NULL, &iconsRect[i]);
            SDL_SetTextureColorMod(iconsText[i], 255,255,255);
        }else{
            cameraSub->RenderCopy(iconsText[i], NULL, &iconsRect[i]);
        }
    }
    cameraSub->RenderCopy(frameTexture, NULL, &frameRect);
    if(playerPlaced && doorPlaced){
        SDL_SetTextureColorMod(createText, 255,255,255);
        if(createSelected) cameraSub->RenderCopy(pointer, NULL, &pointerRect);
    }else{
        SDL_SetTextureColorMod(createText, 192,192,192);
    }
    //
    cameraSub->RenderCopy(createText, NULL, &createRect);
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if(nameInputPointerClicked) cameraSub->RenderCopyEx(pointer, NULL, &nameInputPointer, 180.0, NULL, flip);
    SDL_SetRenderDrawColor(renderer2, 255,255,255,255);
    SDL_RenderFillRect(renderer2, &nameInputRect);
    SDL_SetRenderDrawColor(renderer2, 128,128,128,255);

    cameraSub->RenderCopy(levelNameText, NULL, &levelNameRect);

    SDL_Surface* tmp = TTF_RenderText_Solid(returnFont24, levelNameBuffer.c_str(), blackColor);
    SDL_Texture* txt = SDL_CreateTextureFromSurface(renderer2, tmp);
    SDL_FreeSurface(tmp);
    SDL_Rect toDisp;
    SDL_QueryTexture(txt, NULL, NULL, &toDisp.w, &toDisp.h);
    toDisp.x = nameInputRect.x +5;
    toDisp.y = nameInputRect.y +5;
    cameraSub->RenderCopy(txt, NULL, &toDisp);
    SDL_DestroyTexture(txt);
    ///////////////////////////////////////////////

    SDL_RenderPresent(renderer2);
    SDL_RenderPresent(renderer);


}


