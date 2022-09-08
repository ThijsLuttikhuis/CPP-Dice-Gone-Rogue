//
// Created by thijs on 08-09-22.
//

#ifndef DICEGONEROGUE_FACESHIELDANDMANA_H
#define DICEGONEROGUE_FACESHIELDANDMANA_H

#include "Face.h"

namespace DGR {

class FaceShieldAndMana : public Face {
private:

public:
    explicit FaceShieldAndMana(int face_) : Face(face_) {};

    FaceShieldAndMana(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, value, bonusValuePerLevel, modifiers) {}

    std::shared_ptr<Face> makeCopy() const override;

    std::string toString() const override;

    bool interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) override;
};

}

#endif //DICEGONEROGUE_FACESHIELDANDMANA_H
