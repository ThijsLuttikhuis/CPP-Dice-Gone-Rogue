//
// Created by thijs on 05-09-22.
//

#include <scene/BattleScene.h>
#include "FaceDamage.h"

namespace DGR {

bool FaceDamage::interactFoe(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) {

    int damage = value;
    if (modifiers.hasModifier(FaceModifier::modifier::first_blood)) {
        if (character->getHP() == character->getMaxHP() && character->getIncomingDamage() == 0) {
            damage *= 2;
        }
    }

    character->setIncomingDamage(character->getIncomingDamage() + damage);
    if (modifiers.hasModifier(FaceModifier::modifier::poison)) {
        character->setIncomingPoison(character->getIncomingPoison() + damage);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applySweepingEdge(character, getSharedFromThis(), battleScene, true);
    }

    return true;
}

std::string FaceDamage::toString() const {
    return "damage";
}

std::shared_ptr<Face> FaceDamage::makeCopy() const {
    return std::make_shared<FaceDamage>(*this);
}

}
