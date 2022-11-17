#include "Turret.h"

#include "../Engine/Game.h"
#include "Orb.h"

Turret::Turret(Game* g, std::string key, float x, float y, float w, float h, uint32_t speed) : Entity(g, key, x, y, w, h){
    time = SDL_GetTicks();
    this->speed = speed;
}

Turret::~Turret(){

}


void Turret::locatePlayer(Player* p){

}

void Turret::update(){
    locatePlayer(game_p->p1);

}

void Turret::render(Camera* camera){

}
