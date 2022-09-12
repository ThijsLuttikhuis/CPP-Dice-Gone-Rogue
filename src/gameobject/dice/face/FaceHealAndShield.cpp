//
// Created by thijs on 09-09-22.
//

#include "FaceHealAndShield.h"
#include "gameobject/Character.h"

namespace DGR {

std::shared_ptr<Face> FaceHealAndShield::makeCopy() const {
    return std::make_shared<FaceHealAndShield>(*this);
}

std::string FaceHealAndShield::toString() const {
    return "heal and shield";
}

bool FaceHealAndShield::interactFriendly(std::shared_ptr<Character> character,
                                         std::shared_ptr<BattleController> battleController) {

    character->addHP(value);
    character->addShield(value);

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