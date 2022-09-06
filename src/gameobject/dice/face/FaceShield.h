//
// Created by thijs on 05-09-22.
//

#ifndef DICEGONEROGUE_FACESHIELD_H
#define DICEGONEROGUE_FACESHIELD_H


#include "Face.h"

namespace DGR {

class FaceShield : public Face {
private:

public:
    explicit FaceShield(int face_) : Face(face_) {};

    FaceShield(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, value, bonusValuePerLevel, modifiers) {}

    std::shared_ptr<Face> makeCopy() const override;

    std::string toString() const override;

    bool interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) override;
};

}


#endif //DICEGONEROGUE_FACESHIELD_H
