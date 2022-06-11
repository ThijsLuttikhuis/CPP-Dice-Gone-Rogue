//
// Created by thijs on 10-06-22.
//

#ifndef DICEGONEROGUE_SPELLTYPE_H
#define DICEGONEROGUE_SPELLTYPE_H

#include <utilities/BiDirectionalMap.h>

namespace DGR {

class SpellType {
public:
    enum spellType : int {
        empty,
        damage,
        heal,
        damage_or_shield,
        heal_or_shield,
        damage_if_full_health,
        kill_if_below_threshold,
    };
private:
    static BiDirectionalMap<std::string, spellType> stringsAndSpellTypes;

    spellType type;
public:
    SpellType() : type(spellType::empty) {}

    explicit SpellType(const spellType &type) : type(type) {};

    /// getters
    [[nodiscard]] const std::string &toString() const;

    [[nodiscard]] spellType getType() const;

    /// overloads
    SpellType &operator =(const spellType &t) {
        type = t;
        return *this;
    }

    bool operator ==(const spellType &t) {
        return t == type;
    }

};

}

#endif //DICEGONEROGUE_SPELLTYPE_H
