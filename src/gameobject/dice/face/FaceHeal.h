//
// Created by thijs on 06-09-22.
//

#ifndef DICEGONEROGUE_FACEHEAL_H
#define DICEGONEROGUE_FACEHEAL_H


#include "Face.h"
#include <scene/BattleScene.h>

namespace DGR {

class FaceHeal : public Face {
private:

public:
    explicit FaceHeal(int face_) : Face(face_) {};

    FaceHeal(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, value, bonusValuePerLevel, modifiers) {}

    std::shared_ptr<Face> makeCopy() const override;

    std::string toString() const override;

    bool interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleController> battleController) override;
};

}


#endif //DICEGONEROGUE_FACEHEAL_H
