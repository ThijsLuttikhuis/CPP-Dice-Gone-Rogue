//
// Created by thijs on 06-09-22.
//

#include "FaceEmpty.h"

namespace DGR {

std::shared_ptr<Face> FaceEmpty::makeCopy() const {
    return std::make_shared<FaceEmpty>(*this);
}

std::string FaceEmpty::getToolTipString() const {
    return toString();
}

std::string FaceEmpty::toString() const {
    return "empty";
}

bool FaceEmpty::interactSelf(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene) {
    (void) character, (void) battleScene;

    return true;
}

void FaceEmpty::setValue(int value_) {
    (void) value_;
}

void FaceEmpty::setBonusValuePerLevel(int bonus) {
    (void) bonus;
}

void FaceEmpty::addModifier(FaceModifier::modifier modifier) {
    (void) modifier;
}

void FaceEmpty::addModifier(const std::string &modifierStr) {
    (void) modifierStr;
}

void FaceEmpty::removeModifier(FaceModifier::modifier modifier) {
    (void) modifier;
}

void FaceEmpty::setModifiers(unsigned int modifiers_) {
    (void) modifiers_;
}

void FaceEmpty::setModifiers(FaceModifier modifiers_) {
    (void) modifiers_;
}

}
