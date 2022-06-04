//
// Created by thijs on 04-06-22.
//

#include <iostream>
#include "FaceModifier.h"

namespace DGR {

FaceModifier::modifier FaceModifier::stringToModifier(const std::string &modifierStr) {
    if (modifierStr == "ranged") {
        return modifier::ranged;
    }
    if (modifierStr == "sweeping_edge") {
        return modifier::sweeping_edge;
    }
    if (modifierStr == "single_use") {
        return modifier::single_use;
    }
    if (modifierStr == "poison") {
        return modifier::poison;
    }
    if (modifierStr == "cleanse") {
        return modifier::cleanse;
    }
    if (modifierStr == "first_blood") {
        return modifier::first_blood;
    }
    if (modifierStr == "growth") {
        return modifier::growth;
    }
    if (modifierStr == "decay") {
        return modifier::decay;
    }
    std::cerr << "FaceModifier::stringToModifier: error: string is unknown: " << modifierStr << std::endl;
    return modifier::none;
}

bool FaceModifier::hasModifier(modifier modifier) {
    return modifiers & static_cast<unsigned int>(modifier);
}

unsigned int FaceModifier::getModifiers() {
    return modifiers;
}

void FaceModifier::addModifier(const std::string &modifierStr) {
    modifier modifier_ = stringToModifier(modifierStr);
    if (!hasModifier(modifier_)) {
        modifiers += static_cast<unsigned int>(modifier_);
    }
}

void FaceModifier::addModifier(modifier modifier_) {
    if (!hasModifier(modifier_)) {
        modifiers += static_cast<unsigned int>(modifier_);
    }
}


glm::vec3 FaceModifier::toColor() {
    if (modifiers == 0) {
        return glm::vec3(1.0f);
    }

    if (modifiers & static_cast<unsigned int>(modifier::ranged)) {
        return glm::vec3(1.0f, 0.4f, 0.4f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::sweeping_edge)) {
        return glm::vec3(4.0f, 4.5f, 1.0f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::single_use)) {
        return glm::vec3(0.4f, 0.1f, 0.7f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::poison)) {
        return glm::vec3(0.1f, 0.5f, 0.1f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::cleanse)) {
        return glm::vec3(0.8f, 1.0f, 0.7f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::first_blood)) {
        return glm::vec3(0.0f, 0.9f, 0.7f);
    }

    return glm::vec3(1.0f);
}


}
