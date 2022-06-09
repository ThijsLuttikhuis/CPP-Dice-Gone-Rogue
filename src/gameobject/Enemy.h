//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_ENEMY_H
#define DICEGONEROGUE_ENEMY_H

#include <utility>

#include "Character.h"

namespace DGR {

class Dice;

class Enemy : public Character {
public:
    explicit Enemy(std::string name) : Character(std::move(name)) {};

    Enemy(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    [[nodiscard]] std::string getCharacterType() const override;
};

}

#endif //DICEGONEROGUE_ENEMY_H
