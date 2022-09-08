//
// Created by thijs on 08-09-22.
//

#include "FaceShieldAndMana.h"
#include "gameobject/Character.h"
#include "scene/BattleScene.h"

namespace DGR {

std::shared_ptr<Face> FaceShieldAndMana::makeCopy() const {
    return std::make_shared<FaceShieldAndMana>(*this);
}

std::string FaceShieldAndMana::toString() const {
    return "shield and mana";
}

bool FaceShieldAndMana::interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) {

    character->setShield(character->getShield() + value);
    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        character->setPoison(0);
        character->setIncomingPoison(0);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
        applySweepingEdge(character, getSharedFromThis(), battleScene, false);
    }

    battleScene->addMana(value);

    return true;
}

}