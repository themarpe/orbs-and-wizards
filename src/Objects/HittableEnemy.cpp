#include "HittableEnemy.h"

#include <Game.h>

HittableEnemy::HittableEnemy(Game* g, std::string key, float x, float y, float w, float h) : Entity(g, key, x, y, w, h) {
    healthPoints = 1;
};

void HittableEnemy::hit(int damage) {
    healthPoints -= damage;
}

HittableEnemy::HittableEnemy() {}
HittableEnemy::~HittableEnemy() {
    // dtor
}
