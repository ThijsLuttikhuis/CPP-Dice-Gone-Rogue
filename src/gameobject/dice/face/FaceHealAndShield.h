//
// Created by thijs on 09-09-22.
//

#ifndef DICEGONEROGUE_FACEHEALANDSHIELD_H
#define DICEGONEROGUE_FACEHEALANDSHIELD_H


#include "Face.h"

namespace DGR {

class FaceHealAndShield : public Face {
private:

public:
    explicit FaceHealAndShield(int face_) : Face(face_) {};

    FaceHealAndShield(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, value, bonusValuePerLevel, modifiers) {}

    std::shared_ptr<Face> makeCopy() const override;

    std::string toString() const override;

    bool interactFriendly(std::shared_ptr<Character> character,
                          std::shared_ptr<BattleController> battleController) override;
};

}


#endif //DICEGONEROGUE_FACEHEALANDSHIELD_H
