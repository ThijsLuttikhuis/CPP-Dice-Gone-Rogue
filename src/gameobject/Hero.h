//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_HERO_H
#define DICEGONEROGUE_HERO_H

#include <utility>

#include "Character.h"

namespace DGR {

class Dice;

class Hero : public Character {

public:
    explicit Hero(std::string name) : Character(std::move(name)) {};

    Hero(const std::string &textureName, glm::vec2 position, glm::vec2 size);

};

}

#endif //DICEGONEROGUE_HERO_H
