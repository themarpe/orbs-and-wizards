#include "Game.h"

#include <GameEngine.h>
#include <States/GameOverState.h>
#include <States/GameState.h>
#include <States/GameWonState.h>
#include <States/PauseState.h>
#include <UI.h>

#include "../Objects/Block.h"
#include "../Objects/Entity.h"
#include "../Objects/Orb.h"
#include "../Objects/Player.h"
#include "Camera.h"
#include "Level.h"
#include "SpriteSheet.h"

std::map<std::string, SpriteSheet> Game::SPRITESHEET;

// STATICS
bool Game::checkBoxCollision(SDL_Rect* a, SDL_Rect* b) {
    if(a->x + a->w > b->x && a->x < b->x + b->w && a->y + a->h > b->y && a->y < b->y + b->h) {
        return true;
    } else {
        return false;
    }
}

Game::Game(GameEngine* gameEngine) {
    typeOfGame = TypeOfGame::PLAYABLE;

    this->gameEngine = gameEngine;
    renderer = gameEngine->renderer;

    window = gameEngine->window;
    screenWidth = gameEngine->screenWidth;
    screenHeight = gameEngine->screenHeight;

    levelPath = gameEngine->getLevelPath();
    level = NULL;

    p1 = new Player(this, "PlayerBlue", 0, 0, 64, 64, 6);
    camera = new Camera(renderer, 0.0, 0.0, screenWidth, screenHeight);
    level = new Level(this, levelPath);
    ui = new UI(renderer);
    ui->setHealthPoints(6);
    ui->setMana(0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    std::string title = "ORBS AND WIZARDS | " + level->getLevelName();  // + " | FPS: XX";
    SDL_SetWindowTitle(window, title.c_str());

    std::cout << "inited game! \n";

    std::cout << "AFTER INIT , PLAYER POINTER = " << p1 << "\n";
}

Game::Game(GameEngine* gameEngine, std::string replayFile) {
    typeOfGame = TypeOfGame::REPLAY;

    std::cout << "set typeOfGame = REPLAY \n\n";

    replayInputStream.open(replayFile.c_str(), std::ios::in | std::ios::binary);
    if(!replayInputStream.is_open()) {
        gameEngine->changeState(MainMenuState::getInstance());

        std::cout << "error = REPLAY \n\n";

        return;
    }

    replayInputStream.seekg(0, std::ios::beg);

    // read until \n -> levelName;
    std::string replayLevel;
    getline(replayInputStream, replayLevel);

    std::cout << replayLevel << " fetched replay level name xxx.lvl\n\n";

    // start of replay caching
    long long replaySeed;
    replayInputStream.read((char*)&replaySeed, sizeof(replaySeed));

    std::cout << "rngSeed = " << replaySeed << "\n";
    gameEngine->setRngSeed(replaySeed);
    long long framesRecorded;
    replayInputStream.read((char*)&framesRecorded, sizeof(framesRecorded));
    std::cout << framesRecorded << " <- recorded frames\n";

    while(framesRecorded-- > 0) {
        ReplayStruct replayFrame;
        int numberOfCommands;
        replayInputStream.read((char*)&replayFrame.frameNumber, sizeof(long long));
        replayInputStream.read((char*)&numberOfCommands, sizeof(numberOfCommands));

        std::cout << " frame number = " << replayFrame.frameNumber << " num commands = " << numberOfCommands << " ";
        while(numberOfCommands-- > 0) {
            char tmpCommand;
            replayInputStream.read((char*)&tmpCommand, 1);
            InputCommands cmd = static_cast<InputCommands>(tmpCommand);
            replayFrame.inputCommands.push_back(cmd);
            std::cout << tmpCommand << " ";
        }
        replayCache.push(replayFrame);
        std::cout << "\n";
    }
    // end of replay caching

    this->gameEngine = gameEngine;
    renderer = gameEngine->renderer;

    window = gameEngine->window;
    screenWidth = gameEngine->screenWidth;
    screenHeight = gameEngine->screenHeight;

    level = NULL;

    p1 = new Player(this, "PlayerBlue", 0, 0, 64, 64, 6);
    camera = new Camera(renderer, 0.0, 0.0, screenWidth, screenHeight);
    level = new Level(this, replayLevel);
    ui = new UI(renderer);
    ui->setHealthPoints(6);
    ui->setMana(0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    std::string title = "ORBS AND WIZARDS | " + level->getLevelName();  // + " | FPS: XX";
    SDL_SetWindowTitle(window, title.c_str());

    std::cout << "inited replay game! \n";

    std::cout << "AFTER INIT , PLAYER POINTER = " << p1 << "\n";
}

Game::~Game() {
    delete p1;
    delete camera;
    delete level;
    delete ui;

    for(auto e : entities) delete e;
    entities.clear();

    while(!entitiesToAdd.empty()) {
        delete entitiesToAdd.top();
        entitiesToAdd.pop();
    }
}

void Game::handleInput() {
    if(typeOfGame == TypeOfGame::PLAYABLE) {
        ReplayStruct tmpReplayStruct;
        tmpReplayStruct.frameNumber = frameCounter;

        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            InputCommands command = InputCommands::NOP;

            if(e.quit.type == SDL_QUIT) {
                gameEngine->quit();
            } else if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
                switch(e.key.keysym.sym) {
                    case SDLK_a:
                        command = InputCommands::SDLK_a_DOWN;
                        break;
                    case SDLK_d:
                        command = InputCommands::SDLK_d_DOWN;
                        break;
                    case SDLK_w:
                        command = InputCommands::SDLK_w_DOWN;
                        break;
                    case SDLK_SPACE:
                        command = InputCommands::SDLK_SPACE_DOWN;
                        break;
                    case SDLK_1:
                        command = InputCommands::SDLK_1_DOWN;
                        break;
                    case SDLK_2:
                        command = InputCommands::SDLK_2_DOWN;
                        break;
                    case SDLK_3:
                        command = InputCommands::SDLK_3_DOWN;
                        break;
                    case SDLK_q:
                        command = InputCommands::SDLK_q_DOWN;
                        break;
                    case SDLK_e:
                        command = InputCommands::SDLK_e_DOWN;
                        break;
                }

            }

            else if(e.type == SDL_KEYUP) {
                switch(e.key.keysym.sym) {
                    case SDLK_a:
                        command = InputCommands::SDLK_a_UP;
                        break;
                    case SDLK_d:
                        command = InputCommands::SDLK_d_UP;
                        break;
                    case SDLK_w:
                        command = InputCommands::SDLK_w_UP;
                        break;
                    case SDLK_SPACE:
                        command = InputCommands::SDLK_SPACE_UP;
                        break;
                    case SDLK_1:
                        command = InputCommands::SDLK_1_UP;
                        break;
                    case SDLK_2:
                        command = InputCommands::SDLK_2_UP;
                        break;
                    case SDLK_3:
                        command = InputCommands::SDLK_3_UP;
                        break;
                    case SDLK_q:
                        command = InputCommands::SDLK_q_UP;
                        break;
                    case SDLK_e:
                        command = InputCommands::SDLK_e_UP;
                        break;

                    case SDLK_ESCAPE:
                        command = InputCommands::SDLK_ESCAPE_UP;
                        break;

                    case SDLK_k:
                        command = InputCommands::SDLK_k_UP;
                        break;
                }
            }

            // react to the command
            switch(command) {
                case InputCommands::SDLK_a_DOWN:
                    p1->keys[(int)Player::Keys::LEFT] = true;
                    break;
                case InputCommands::SDLK_d_DOWN:
                    p1->keys[(int)Player::Keys::RIGHT] = true;
                    break;
                case InputCommands::SDLK_w_DOWN:
                    p1->keys[(int)Player::Keys::UP] = true;
                    break;
                case InputCommands::SDLK_SPACE_DOWN:
                    p1->keys[(int)Player::Keys::SPACE] = true;
                    break;
                case InputCommands::SDLK_1_DOWN:
                    p1->keys[(int)Player::Keys::NUM1] = true;
                    break;
                case InputCommands::SDLK_2_DOWN:
                    p1->keys[(int)Player::Keys::NUM2] = true;
                    break;
                case InputCommands::SDLK_3_DOWN:
                    p1->keys[(int)Player::Keys::NUM3] = true;
                    break;
                case InputCommands::SDLK_q_DOWN:
                    p1->keys[(int)Player::Keys::Q] = true;
                    break;
                case InputCommands::SDLK_e_DOWN:
                    p1->keys[(int)Player::Keys::E] = true;
                    break;

                case InputCommands::SDLK_a_UP:
                    p1->keys[(int)Player::Keys::LEFT] = false;
                    break;
                case InputCommands::SDLK_d_UP:
                    p1->keys[(int)Player::Keys::RIGHT] = false;
                    break;
                case InputCommands::SDLK_w_UP:
                    p1->keys[(int)Player::Keys::UP] = false;
                    break;
                case InputCommands::SDLK_SPACE_UP:
                    p1->keys[(int)Player::Keys::SPACE] = false;
                    break;
                case InputCommands::SDLK_1_UP:
                    p1->keys[(int)Player::Keys::NUM1] = false;
                    break;
                case InputCommands::SDLK_2_UP:
                    p1->keys[(int)Player::Keys::NUM2] = false;
                    break;
                case InputCommands::SDLK_3_UP:
                    p1->keys[(int)Player::Keys::NUM3] = false;
                    break;
                case InputCommands::SDLK_q_UP:
                    p1->keys[(int)Player::Keys::Q] = false;
                    break;
                case InputCommands::SDLK_e_UP:
                    p1->keys[(int)Player::Keys::E] = false;
                    break;

                case InputCommands::SDLK_ESCAPE_UP:
                    gameEngine->pushState(PauseState::getInstance());
                    break;

                case InputCommands::SDLK_k_UP:
                    nextLevel();
                    break;
            }

            if(command != InputCommands::NOP) tmpReplayStruct.inputCommands.push_back(command);
        }

        if(tmpReplayStruct.inputCommands.size() > 0) replayCache.push(tmpReplayStruct);

    } else if(typeOfGame == TypeOfGame::REPLAY) {
        ReplayStruct frontReplayStruct = replayCache.front();
        if(frameCounter >= frontReplayStruct.frameNumber) {
            for(InputCommands inputCommand : frontReplayStruct.inputCommands) {
                switch(inputCommand) {
                    case InputCommands::SDLK_a_DOWN:
                        p1->keys[(int)Player::Keys::LEFT] = true;
                        break;
                    case InputCommands::SDLK_d_DOWN:
                        p1->keys[(int)Player::Keys::RIGHT] = true;
                        break;
                    case InputCommands::SDLK_w_DOWN:
                        p1->keys[(int)Player::Keys::UP] = true;
                        break;
                    case InputCommands::SDLK_SPACE_DOWN:
                        p1->keys[(int)Player::Keys::SPACE] = true;
                        break;
                    case InputCommands::SDLK_1_DOWN:
                        p1->keys[(int)Player::Keys::NUM1] = true;
                        break;
                    case InputCommands::SDLK_2_DOWN:
                        p1->keys[(int)Player::Keys::NUM2] = true;
                        break;
                    case InputCommands::SDLK_3_DOWN:
                        p1->keys[(int)Player::Keys::NUM3] = true;
                        break;
                    case InputCommands::SDLK_q_DOWN:
                        p1->keys[(int)Player::Keys::Q] = true;
                        break;
                    case InputCommands::SDLK_e_DOWN:
                        p1->keys[(int)Player::Keys::E] = true;
                        break;

                    case InputCommands::SDLK_a_UP:
                        p1->keys[(int)Player::Keys::LEFT] = false;
                        break;
                    case InputCommands::SDLK_d_UP:
                        p1->keys[(int)Player::Keys::RIGHT] = false;
                        break;
                    case InputCommands::SDLK_w_UP:
                        p1->keys[(int)Player::Keys::UP] = false;
                        break;
                    case InputCommands::SDLK_SPACE_UP:
                        p1->keys[(int)Player::Keys::SPACE] = false;
                        break;
                    case InputCommands::SDLK_1_UP:
                        p1->keys[(int)Player::Keys::NUM1] = false;
                        break;
                    case InputCommands::SDLK_2_UP:
                        p1->keys[(int)Player::Keys::NUM2] = false;
                        break;
                    case InputCommands::SDLK_3_UP:
                        p1->keys[(int)Player::Keys::NUM3] = false;
                        break;
                    case InputCommands::SDLK_q_UP:
                        p1->keys[(int)Player::Keys::Q] = false;
                        break;
                    case InputCommands::SDLK_e_UP:
                        p1->keys[(int)Player::Keys::E] = false;
                        break;

                    case InputCommands::SDLK_ESCAPE_UP:
                        gameEngine->pushState(PauseState::getInstance());
                        break;

                    case InputCommands::SDLK_k_UP:
                        nextLevel();
                        break;
                }
            }

            replayCache.pop();
        }
    }

    p1->handleInput();
}

void Game::addEntity(Entity* entity) {
    entitiesToAdd.push(entity);
}

void Game::nextLevel() {
    nextLevelBool = true;
}

void Game::loadNextLevel() {
    if(gameEngine->getLevelType() == GameEngine::LevelType::NORMAL) {
        int cl = gameEngine->getCurrentLevel();
        if(cl < gameEngine->getMaxLevels()) {
            cl++;
            std::string lvlPath = "Levels/level0";
            lvlPath += (char)(cl + 48);
            lvlPath += ".lvl";
            std::cout << "LVL PATH = " << lvlPath << "\n";
            gameEngine->setLevelPath(lvlPath);
            gameEngine->setCurrentLevel(cl);
            gameEngine->changeState(GameState::getInstance());
        } else {
            gameEngine->setLevelPath("levels/level00.lvl");
            gameEngine->setCurrentLevel(0);
            gameEngine->pushState(GameWonState::getInstance());
        }
    } else if(gameEngine->getLevelType() == gameEngine->LevelType::CUSTOM) {
        gameEngine->pushState(GameWonState::getInstance());
    } else if(gameEngine->getLevelType() == gameEngine->LevelType::REPLAY) {
        gameEngine->changeState(MainMenuState::getInstance());
    }
}

void Game::saveReplay() {
    // save replay (only for testing) append with time
    std::string replayName = level->getLevelName() + "_" + GameEngine::getDateTime() + ".oawr";
    std::cout << replayName << "<-replay name\n";
    replayOutputStream.open("Replays/" + replayName, std::ios::out | std::ios::binary);
    if(replayOutputStream.is_open()) {
        replayOutputStream << gameEngine->getLevelPath() << "\n";  // level filename
        long long replaySeed = gameEngine->getRngSeed();
        long long numFrames = replayCache.size();
        replayOutputStream.write((char*)&replaySeed, sizeof(replaySeed));
        replayOutputStream.write((char*)&numFrames, sizeof(numFrames));
        std::cout << "saving... soo the seed is = " << gameEngine->getRngSeed() << " frames recorded = " << replayCache.size() << " <- thats it, tnx\n";

        while(!replayCache.empty()) {
            ReplayStruct tmpReplayStruct = replayCache.front();
            std::cout << "writing frame by frame: frameNumber = " << tmpReplayStruct.frameNumber << "\n";
            replayOutputStream.write((char*)&tmpReplayStruct.frameNumber, sizeof(tmpReplayStruct.frameNumber));
            int numInputCommands = tmpReplayStruct.inputCommands.size();
            replayOutputStream.write((char*)&numInputCommands, sizeof(numInputCommands));

            for(InputCommands command : tmpReplayStruct.inputCommands) {
                char cmd = static_cast<char>(command);
                replayOutputStream.write(&cmd, 1);
            }
            replayCache.pop();
        }

        replayOutputStream.close();
    }
}

void Game::gameOver() {
    if(gameEngine->getLevelType() == gameEngine->LevelType::REPLAY) {
        gameEngine->changeState(MainMenuState::getInstance());
    } else {
        gameEngine->pushState(GameOverState::getInstance());
    }
}

void Game::update() {
    level->update();
    if(p1) p1->update();

    for(auto it = entities.begin(); it != entities.end();) {
        if((*it)->getAlive() == false) {
            delete(*it);
            (*it) = nullptr;
            it = entities.erase(it);
        } else {
            (*it)->update();
            ++it;
        }
    }

    while(entitiesToAdd.size() > 0) {
        entities.push_back(entitiesToAdd.top());
        entitiesToAdd.pop();
    }

    if(!p1->getAlive()) gameOver();
    if(nextLevelBool) loadNextLevel();

    ++frameCounter;
}

void Game::render() {
    // rendering on camera
    level->render(camera);
    for(auto e : entities) e->render(camera);
    if(p1) p1->render(camera);

    // RENDERING ON SCREEN (UI)
    ui->render(renderer);

    // for(auto e:Game::SPRITESHEET){
    //  std::cout << "text name = " << e.first << " text pointer = " << e.second.getSheetTexture() << "\n";
    //}
}
