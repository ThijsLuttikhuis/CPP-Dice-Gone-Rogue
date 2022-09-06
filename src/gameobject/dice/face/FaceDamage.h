//
// Created by thijs on 05-09-22.
//

#ifndef DICEGONEROGUE_FACEDAMAGE_H
#define DICEGONEROGUE_FACEDAMAGE_H


#include "Face.h"

namespace DGR {

class FaceDamage : public Face {
private:

public:
    explicit FaceDamage(int face_) : Face(face_) {};

    FaceDamage(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, value, bonusValuePerLevel, modifiers) {}

    std::shared_ptr<Face> makeCopy() const override;

    std::string toString() const override;

    bool interactFoe(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) override;
};

}


#endif //DICEGONEROGUE_FACEDAMAGE_H
