//
// Created by thijs on 04-06-22.
//

#include "Enemy.h"
#include "spell/Spell.h"

namespace DGR {

std::string Enemy::getCharacterType() const {
    return "enemy";
}

Enemy* Enemy::makeCopy(bool copyUniqueID) const {
    auto* copy = new Enemy(name);
    Character::setCopyParameters(copy);
    if (copyUniqueID) {
        copy->setUniqueID(getUniqueID());
    }
    return copy;
}

}