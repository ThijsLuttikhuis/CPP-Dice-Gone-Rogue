//
// Created by thijs on 04-06-22.
//

#include "Enemy.h"

namespace DGR {

Enemy::Enemy(const std::string &name, glm::vec2 position, glm::vec2 size)
      : Character(name, position, size) {
}

std::string Enemy::getCharacterType() const {
    return "enemy";
}

}