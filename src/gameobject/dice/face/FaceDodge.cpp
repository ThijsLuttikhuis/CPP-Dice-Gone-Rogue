//
// Created by thijs on 08-09-22.
//

#include "FaceDodge.h"
#include "gameobject/Character.h"

namespace DGR {


std::string FaceDodge::getToolTipString() const {
    std::ostringstream tooltipOSS;
    tooltipOSS << toString();

    auto modStr = modifiers.toString();
    if (!modStr.empty()) {
        tooltipOSS << " (^" << modStr << "^)";
    }
    std::string toolTipString = tooltipOSS.str();
    return toolTipString;
}

std::string FaceDodge::toString() const {
    return "dodge";
}

std::shared_ptr<Face> FaceDodge::makeCopy() const {
    return std::make_shared<FaceDodge>(*this);
}

bool FaceDodge::interactSelf(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) {
    (void) battleScene;

    character->setDodging(true);

    return true;
}

}