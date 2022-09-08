//
// Created by thijs on 08-09-22.
//

#include "FaceMana.h"

namespace DGR {


std::shared_ptr<Face> FaceMana::makeCopy() const {
    return std::make_shared<FaceMana>(*this);
}

std::string FaceMana::toString() const {
    return "mana";
}

bool FaceMana::interactSelf(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) {
    (void)character;

    battleScene->addMana(value);
    return true;
}
}