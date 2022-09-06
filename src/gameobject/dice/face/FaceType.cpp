//
// Created by thijs on 04-06-22.
//

#include "FaceType.h"

namespace DGR {

BiDirectionalMap<std::string, FaceType::faceType> FaceType::stringsAndFaceTypes =
      BiDirectionalMap(std::vector<std::pair<std::string, FaceType::faceType>>{
            {"empty",                  empty},
            {"damage",                 damage},
            {"mana",                   mana},
            {"heal",                   heal},
            {"bonus health",           bonus_health},
            {"shield",                 shield},
            {"dodge",                  dodge},
            {"undying",                undying},
            {"heal and shield",        heal_and_shield},
            {"heal and mana",          heal_and_mana},
            {"damage and mana",          damage_and_mana},
            {"damage and self shield", damage_and_self_shield},
            {"shield and mana",        shield_and_mana}}
      );

const std::string &FaceType::toString() const {
    return stringsAndFaceTypes.at(type);
}

FaceType::faceType FaceType::getType() const {
    return type;
}

}
