#include <GameEngine.h>
#include <States/State.h>

void GameEngine::init() {
    setRngSeed(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    running = true;

    screenWidth = 800;
    screenHeight = 640;

    window = SDL_CreateWindow("ORBS AND WIZARDS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, NULL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL || window == NULL) {
        std::cout << "fatal error, exiting\n";
    }

    std::cout << "window pointer = " << window << "\n";

    // RECT AND VEC FOR COPYING
    SDL_Rect r;
    std::vector<SDL_Rect> v;

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 6; j++) {
            r.x = j * 32;
            r.y = i * 32;
            r.w = 32;
            r.h = 32;
            v.push_back(r);
        };
    Game::SPRITESHEET.insert(std::make_pair("PlayerBlue", SpriteSheet("res/Objects/player1.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 6; i++) {
        r.x = i * 64;
        r.y = 0;
        r.w = 64;
        r.h = 64;
        v.push_back(r);
    }
    Game::SPRITESHEET.insert(std::make_pair("HealAnimation", SpriteSheet("res/Objects/healSparkle.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 6; i++) {
        r.x = i * 64;
        r.y = 0;
        r.w = 64;
        r.h = 64;
        v.push_back(r);
    }
    Game::SPRITESHEET.insert(std::make_pair("ManaAnimation", SpriteSheet("res/Objects/manaSparkle.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 6; i++) {
        r.x = i * 64;
        r.y = 0;
        r.w = 64;
        r.h = 64;
        v.push_back(r);
    }
    Game::SPRITESHEET.insert(std::make_pair("RuneAnimation", SpriteSheet("res/Objects/runeSparkle.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 7; i++)
        for(int j = 0; j < 8; j++) {
            r.x = j * 64;
            r.y = i * 64;
            r.w = 64;
            r.h = 64;
            v.push_back(r);
        };
    Game::SPRITESHEET.insert(std::make_pair("FireOrb2", SpriteSheet("res/Objects/fireorb.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 7; i++)
        for(int j = 0; j < 8; j++) {
            r.x = j * 128;
            r.y = i * 128;
            r.w = 128;
            r.h = 128;
            v.push_back(r);
        };
    Game::SPRITESHEET.insert(std::make_pair("IceOrb2", SpriteSheet("res/Objects/iceorb2.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 7; i++)
        for(int j = 0; j < 8; j++) {
            r.x = j * 64;
            r.y = i * 64;
            r.w = 64;
            r.h = 64;
            v.push_back(r);
        };
    Game::SPRITESHEET.insert(std::make_pair("IceOrb3", SpriteSheet("res/Objects/iceorb3.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 8; j++) {
            r.x = j * 16;
            r.y = i * 32;
            r.w = 16;
            r.h = 32;
            v.push_back(r);
        };
    Game::SPRITESHEET.insert(std::make_pair("SkeletonWhite", SpriteSheet("res/Objects/skeletonWhite.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 8; i++) {
        r.x = i * 16;
        r.y = 0;
        r.w = 16;
        r.h = 16;
        v.push_back(r);
    }
    Game::SPRITESHEET.insert(std::make_pair("HealthPotion", SpriteSheet("res/Objects/healthPotion.png", v, renderer)));
    v.clear();

    for(int i = 0; i < 8; i++) {
        SDL_Rect r;
        r.x = i * 16;
        r.y = 0;
        r.w = 16;
        r.h = 16;
        v.push_back(r);
    }
    Game::SPRITESHEET.insert(std::make_pair("ManaPotion", SpriteSheet("res/Objects/manaPotion.png", v, renderer)));
    v.clear();

    r.x = 0;
    r.y = 0;
    r.w = 256;
    r.h = 256;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("Fire2Rune", SpriteSheet("res/Objects/runeFireorb2.png", v, renderer)));
    v.clear();

    r.x = 0;
    r.y = 0;
    r.w = 256;
    r.h = 256;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("Fire3Rune", SpriteSheet("res/Objects/runeFireorb3.png", v, renderer)));
    v.clear();

    r.x = 0;
    r.y = 0;
    r.w = 256;
    r.h = 256;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("Ice2Rune", SpriteSheet("res/Objects/runeIceorb2.png", v, renderer)));
    v.clear();

    r.x = 0;
    r.y = 0;
    r.w = 256;
    r.h = 256;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("Ice3Rune", SpriteSheet("res/Objects/runeIceorb3.png", v, renderer)));
    v.clear();

    r.x = 0;
    r.y = 0;
    r.w = 64;
    r.h = 64;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("RuneKey", SpriteSheet("res/Objects/runeKey2.png", v, renderer)));
    v.clear();

    r.x = 0;
    r.y = 0;
    r.w = 64;
    r.h = 64;
    v.push_back(r);
    Game::SPRITESHEET.insert(std::make_pair("Door", SpriteSheet("res/Objects/door.png", v, renderer)));
    v.clear();

    std::cout << "window pointer = " << window << "\n";

    levelType = LevelType::NORMAL;
    maxLevels = 4;
}

void GameEngine::destroy() {
    // cleanup the all states
    while(!states.empty()) {
        std::cout << "gameengine currently destroying = " << states.top() << "\n";
        states.top()->destroy();
        states.pop();
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void GameEngine::changeState(State* state) {
    // cleanup the current state
    if(!states.empty()) {
        states.top()->destroy();
        states.pop();
    }

    // store and init the new state
    states.push(state);
    states.top()->init(this);
}

void GameEngine::pushState(State* state) {
    // pause current state
    if(!states.empty()) {
        states.top()->pause();
    }

    // store and init the new state
    states.push(state);
    states.top()->init(this);
}

void GameEngine::popState() {
    // cleanup the current state
    if(!states.empty()) {
        states.top()->destroy();
        states.pop();
    }

    // resume previous state
    if(!states.empty()) {
        states.top()->resume();
    }
}

void GameEngine::handleInput() {
    states.top()->handleInput(this);
}

void GameEngine::update() {
    states.top()->update(this);
}

void GameEngine::render() {
    states.top()->render(this);
}

void GameEngine::setLevelPath(std::string s) {
    levelPath = s;
}

std::string GameEngine::getLevelPath() {
    return levelPath;
}

void GameEngine::setReplayPath(std::string s) {
    replayPath = s;
}

std::string GameEngine::getReplayPath() {
    return replayPath;
}
