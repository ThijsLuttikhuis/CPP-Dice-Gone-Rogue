//
// Created by thijs on 06-09-22.
//

#include "FaceHeal.h"

namespace DGR {

std::shared_ptr<Face> FaceHeal::makeCopy() const {
    return std::make_shared<FaceHeal>(*this);
}

std::string FaceHeal::toString() const {
    return "heal";
}

bool FaceHeal::interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) {
    int hp = character->getHP();
    int maxHP = character->getMaxHP();

    hp += value;
    hp = std::min(hp, maxHP);

    character->setHP(hp);

    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        character->setPoison(0);
        character->setIncomingPoison(0);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::regen)) {
        character->setIncomingRegen(character->getIncomingRegen() + value);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
        applySweepingEdge(character, getSharedFromThis(), battleScene, false);
    }

    return true;
}

}
