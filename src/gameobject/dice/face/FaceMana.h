//
// Created by thijs on 08-09-22.
//

#ifndef DICEGONEROGUE_FACEMANA_H
#define DICEGONEROGUE_FACEMANA_H


#include "Face.h"
#include <scene/BattleScene.h>

namespace DGR {

class FaceMana : public Face {
private:

public:
    explicit FaceMana(int face_) : Face(face_) {};

    FaceMana(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, value, bonusValuePerLevel, modifiers) {}

    std::shared_ptr<Face> makeCopy() const override;

    std::string toString() const override;

    bool interactSelf(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) override;
};

}



#endif //DICEGONEROGUE_FACEMANA_H
