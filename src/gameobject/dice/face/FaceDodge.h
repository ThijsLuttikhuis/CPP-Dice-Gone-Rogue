//
// Created by thijs on 08-09-22.
//

#ifndef DICEGONEROGUE_FACEDODGE_H
#define DICEGONEROGUE_FACEDODGE_H


#include "Face.h"

namespace DGR {

class FaceDodge : public Face {
private:

public:
    explicit FaceDodge(int face_) : Face(face_) {};

    FaceDodge(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, value, bonusValuePerLevel, modifiers) {}

    std::shared_ptr<Face> makeCopy() const override;

    std::string toString() const override;

    std::string getToolTipString() const override;

    bool interactSelf(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) override;

};

}


#endif //DICEGONEROGUE_FACEDODGE_H
