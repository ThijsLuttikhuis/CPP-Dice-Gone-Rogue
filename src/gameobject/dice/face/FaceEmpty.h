//
// Created by thijs on 06-09-22.
//

#ifndef DICEGONEROGUE_FACEEMPTY_H
#define DICEGONEROGUE_FACEEMPTY_H


#include "Face.h"

namespace DGR {

class FaceEmpty : public Face {
private:

public:
    explicit FaceEmpty(int face_) : Face(face_, 0, 0, FaceModifier()) {};

    FaceEmpty(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : Face(face_, 0, 0, FaceModifier()) {
        (void)value, (void)bonusValuePerLevel, (void)modifiers;
    }

    void setValue(int value_) override;

    void setBonusValuePerLevel(int bonus) override;

    void addModifier(FaceModifier::modifier modifier) override;

    void addModifier(const std::string &modifierStr) override;

    void removeModifier(FaceModifier::modifier modifier) override;

    void setModifiers(unsigned int modifiers_) override;

    void setModifiers(FaceModifier modifiers_) override;

    std::shared_ptr<Face> makeCopy() const override;

    std::string getToolTipString() const override;

    std::string toString() const override;

    bool interactSelf(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) override;

};

}


#endif //DICEGONEROGUE_FACEEMPTY_H
