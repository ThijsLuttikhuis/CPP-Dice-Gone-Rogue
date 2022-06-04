//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_FACETYPE_H
#define DICEGONEROGUE_FACETYPE_H


#include <string>

namespace DGR {

class FaceType {
public:
    enum faceType : int {
        empty,
        damage,
        mana,
        heal,
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
    faceType type;
public:
    FaceType() : type(faceType::empty) {}

    explicit FaceType(const faceType &type) : type(type) {};

    FaceType &operator =(const faceType &t) {
        type = t;
        return *this;
    }

    std::string toString();

};

}

#endif //DICEGONEROGUE_FACETYPE_H
