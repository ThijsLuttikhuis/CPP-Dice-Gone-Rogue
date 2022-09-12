//
// Created by thijs on 05-09-22.
//

#include <scene/BattleScene.h>
#include "FaceDamage.h"
#include "../Dice.h"

namespace DGR {


std::shared_ptr<Face> FaceDamage::makeCopy() const {
    return std::make_shared<FaceDamage>(*this);
}

bool FaceDamage::interactFoe(std::shared_ptr<Character> character, std::shared_ptr<BattleController> battleController) {

    if (modifiers.hasModifier(FaceModifier::modifier::instant)) {
        //TODO: undo to before damage applied step and do this first :x
    }

    int damage = value;
    if (modifiers.hasModifier(FaceModifier::modifier::backstab)) {
        if (character->getHP() == character->getMaxHP() && character->getIncomingDamage() == 0) {
            damage *= 2;
        }
    }

    character->addIncomingDamage(damage);
    if (modifiers.hasModifier(FaceModifier::modifier::poison)) {
        character->addIncomingPoison(damage);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
        applySweepingEdge(character, getSharedFromThis(), battleController, true);
    }

    applyValueModifiers(getSharedFromThis(), character, battleController, true);

    return true;
}

std::string FaceDamage::toString() const {
    return "damage";
}



}
