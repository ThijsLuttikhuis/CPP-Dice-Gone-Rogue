//
// Created by thijs on 04-06-22.
//

#include "FaceType.h"
#include <iostream>

namespace DGR {

std::string FaceType::toString() {
    switch (type) {
        case damage:
            return "damage";
        case mana:
            return "mana";
        case heal:
            return "heal";
        case shield:
            return "shield";
        case dodge:
            return "dodge";
        case undying:
            return "undying";
        case heal_and_shield:
            return "heal_and_shield";
        case heal_and_mana:
            return "heal_and_mana";
        case shield_and_mana:
            return "shield_and_mana";
        case damage_and_mana:
            return "damage_and_mana";
        case damage_and_self_shield:
            return "damage_and_self_shield";
        case empty:
            return "empty";
        default:
            std::cerr << "Face::faceTypeToString: error, type string unknown: " << type << std::endl;
            return "error";
    }
}

}
