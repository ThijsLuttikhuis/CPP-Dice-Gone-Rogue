//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_FACETYPE_H
#define DICEGONEROGUE_FACETYPE_H


#include <string>
#include <utilities/BiDirectionalMap.h>

namespace DGR {

class FaceType {
public:
    enum faceType : int {
        empty,
        damage,
        mana,
        heal,
        bonus_health,
        shield,
        dodge,
        undying,
        heal_and_shield,
        heal_and_mana,
        shield_and_mana,
        damage_and_mana,
        damage_and_self_shield,
    };
private:
    static BiDirectionalMap<std::string, faceType> stringsAndFaceTypes;

    faceType type;
public:
    FaceType() : type(faceType::empty) {}

    explicit FaceType(const faceType &type) : type(type) {};

    /// getters
    [[nodiscard]] const std::string &toString() const;

    [[nodiscard]] faceType getType() const;

    /// overloads
    FaceType &operator =(const faceType &t) {
        type = t;
        return *this;
    }

    bool operator ==(const faceType &t) const {
        return t == type;
    }
    bool operator !=(const faceType &t) const {
        return (t != type);
    }
};

}

#endif //DICEGONEROGUE_FACETYPE_H
