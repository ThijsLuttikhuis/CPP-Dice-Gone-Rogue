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

bool FaceHeal::interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleController> battleController) {

    character->addHP(value);

    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        character->setPoison(0);
        character->setIncomingPoison(0);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::regen)) {
        character->addIncomingRegen(value);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
        applySweepingEdge(character, getSharedFromThis(), battleController, false);
    }

    applyValueModifiers(getSharedFromThis(), character, battleController, true);

    return true;
}

}
