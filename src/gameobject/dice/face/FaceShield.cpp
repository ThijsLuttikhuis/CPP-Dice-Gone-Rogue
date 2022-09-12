//
// Created by thijs on 05-09-22.
//

#include "FaceShield.h"
#include "../../Character.h"

namespace DGR {

std::shared_ptr<Face> FaceShield::makeCopy() const {
    return std::make_shared<FaceShield>(*this);
}

std::string FaceShield::toString() const {
    return "shield";
}

bool FaceShield::interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleController> battleController) {

    character->addShield(value);

    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        character->setPoison(0);
        character->setIncomingPoison(0);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
        applySweepingEdge(character, getSharedFromThis(), battleController, false);
    }

    applyValueModifiers(getSharedFromThis(), character, battleController, true);

    return true;
}

}