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
            {"shield",                 shield},
            {"dodge",                  dodge},
            {"undying",                undying},
            {"heal and shield",        heal_and_shield},
            {"heal and mana",          heal_and_mana},
            {"damage and self shield", damage_and_self_shield},
            {"shield and mana",        shield_and_mana}}
      );

std::string &FaceType::toString() {
    return stringsAndFaceTypes.at(type);
}

FaceType::faceType FaceType::getType() {
    return type;
}

}
