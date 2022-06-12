//
// Created by thijs on 31-05-22.
//

#include "Hero.h"
#include "spell/Spell.h"

namespace DGR {

std::string Hero::getCharacterType() const {
    return "hero";
}

Hero* Hero::makeCopy(bool copyUniqueID) const {
    auto* copy = new Hero(name);
    Character::setCopyParameters(copy);
    if (copyUniqueID) {
        copy->setUniqueID(getUniqueID());
    }
    return copy;
}

}