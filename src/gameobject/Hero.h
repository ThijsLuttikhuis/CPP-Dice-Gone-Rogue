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

    Hero(const std::string &name, glm::vec2 position, glm::vec2 size)
          : Character(name, position, size) {};

    [[nodiscard]] std::string getCharacterType() const override;

    [[nodiscard]] Hero* makeCopy(bool copyUniqueID = false) const;
};

}

#endif //DICEGONEROGUE_HERO_H
