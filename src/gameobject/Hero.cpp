//
// Created by thijs on 31-05-22.
//

#include "Hero.h"
#include "spell/Spell.h"

namespace DGR {

Hero::Hero(const std::string &name, glm::vec2 position, glm::vec2 size)
      : Character(name, position, size) {
}

std::string Hero::getCharacterType() const {
    return "hero";
}

Hero* Hero::makeCopy() const {
    auto* copy = new Hero(name);
    Character::setCopyParameters(copy);
    return copy;
}

}