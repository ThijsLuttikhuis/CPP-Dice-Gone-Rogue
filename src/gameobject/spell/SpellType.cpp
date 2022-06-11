//
// Created by thijs on 10-06-22.
//

#include "SpellType.h"

namespace DGR {

BiDirectionalMap<std::string, SpellType::spellType> SpellType::stringsAndSpellTypes =
      BiDirectionalMap(std::vector<std::pair<std::string, SpellType::spellType>>{
            {"empty",                        empty},
            {"damage",                       damage},
            {"heal",                         heal},
            {"damage or shield",             damage_or_shield},
            {"heal or shield",               heal_or_shield},
            {"damage to an undamaged enemy", damage_if_full_health},
            {"damage if it kills an enemy",  kill_if_below_threshold}}
      );


const std::string &SpellType::toString() const {
    return stringsAndSpellTypes.at(type);
}

SpellType::spellType SpellType::getType() const {
    return type;
}
}
