//
// Created by thijs on 31-05-22.
//

#include "Hero.h"
#include "gameobject/dice/Dice.h"
#include "utilities/Constants.h"
#include <iostream>

namespace DGR {

Hero::Hero(const std::string &name, glm::vec2 position, glm::vec2 size)
      : Character(name, position, size) {
}

}