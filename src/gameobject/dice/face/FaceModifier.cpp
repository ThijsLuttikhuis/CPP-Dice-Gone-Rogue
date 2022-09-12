//
// Created by thijs on 04-06-22.
//

#include <iostream>
#include <vector>
#include <utilities/Utilities.h>

#include "FaceModifier.h"

namespace DGR {

BiDirectionalMap<std::string, FaceModifier::modifier> FaceModifier::stringsAndModifiers =
      BiDirectionalMap(std::vector<std::pair<std::string, FaceModifier::modifier>>{
            {"none",        modifier::none},
            {"ranged",      modifier::ranged},
            {"sweep",       modifier::sweep},
            {"single use",  modifier::single_use},
            {"poison",      modifier::poison},
            {"regenerate",  modifier::regen},
            {"cleanse",     modifier::cleanse},
            {"backstab",    modifier::backstab},
            {"growth",      modifier::growth},
            {"self shield", modifier::self_shield},
            {"instant",     modifier::instant},
            {"mana gain",   modifier::mana_gain},
            {"charged",     modifier::charged},
            {"decay",       modifier::decay}}
      );

std::map<FaceModifier::modifier, glm::vec3> FaceModifier::modifierToColor =
      {{{modifier::none},        {1.0f, 1.0f, 1.0f}},
       {{modifier::ranged},      {0.7f, 0.5f, 0.5f}},
       {{modifier::sweep},       {1.5f, 2.0f, 0.6f}},
       {{modifier::single_use},  {1.0f, 0.5f, 1.0f}},
       {{modifier::poison},      {0.2f, 0.7f, 0.1f}},
       {{modifier::regen},       {0.7f, 0.2f, 0.1f}},
       {{modifier::cleanse},     {0.8f, 1.0f, 0.7f}},
       {{modifier::backstab},    {1.5f, 0.4f, 0.2f}},
       {{modifier::growth},      {1.0f, 1.4f, 1.0f}},
       {{modifier::self_shield}, {0.8f, 0.8f, 0.8f}},
       {{modifier::instant},     {1.3f, 0.6f, 0.6f}},
       {{modifier::mana_gain},   {0.6f, 0.9f, 1.2f}},
       {{modifier::charged},     {0.2f, 0.5f, 1.2f}},
       {{modifier::decay},       {1.0f, 0.8f, 0.8f}}};

FaceModifier::modifier FaceModifier::stringToModifier(const std::string &modifierStr) {
    if (stringsAndModifiers.count(modifierStr)) {
        modifier &mod = stringsAndModifiers.at(modifierStr);
        return mod;
    } else {
        std::cerr << "modifier \"" << modifierStr << "\" does not exist!" << std::endl;
        return modifier::none;
    }

}

std::string FaceModifier::toString() const {
    std::string modString;
    unsigned int maxMod = modifiers;
    for (unsigned int i = 0; i < maxMod; i++) {
        unsigned int m = 1u << i;
        if (m > modifiers) {
            break;
        }
        auto mod = (modifier) m;
        if (hasModifier(mod)) {
            if (!modString.empty()) {
                modString += ", ";
            }
            modString += stringsAndModifiers.at(mod);
        }
    }
    return modString;
}

std::string FaceModifier::toAllModifiersString() const {
    if (getNumberOfModifiers() <= 1) {
        return toString();
    }

    std::string modString;

    auto allMods = getAllModifiers();
    auto allColors = toAllColors();

    for (unsigned long i = 0; i < allColors.size(); i++) {
        modString += "^#" + Utilities::vec32Color(allColors[i]);
        modString += stringsAndModifiers.at(allMods[i]) + "^";
        if (i < allColors.size() - 1) {
            modString += ", ";
        }
    }

    return modString;
}

glm::vec3 FaceModifier::toColor() const {
    return modifierToColor.at(getMainModifier());
}

std::vector<FaceModifier::modifier> FaceModifier::getAllModifiers() const {
    std::vector<FaceModifier::modifier> allMods;
    unsigned int modifiers_ = modifiers;
    while (modifiers_) {
        auto modi = (modifier) (modifiers_ & ~(modifiers_ - 1));
        int modifiersInt_ = (int) modifiers_;
        modifiers_ = (unsigned int) (modifiersInt_ & (modifiersInt_ - 1));
        allMods.push_back(modi);
    }
    return allMods;
}

std::vector<glm::vec3> FaceModifier::toAllColors() const {
    std::vector<glm::vec3> allColors;
    for (auto &mod : getAllModifiers()) {
        allColors.push_back(modifierToColor.at(mod));
    }
    return allColors;
}

unsigned int FaceModifier::getModifiers() const {
    return modifiers;
}

bool FaceModifier::hasModifier(modifier modifier) const {
    return modifiers & static_cast<unsigned int>(modifier);
}

bool FaceModifier::removeModifier(modifier modifier) {
    if (hasModifier(modifier)) {
        modifiers -= static_cast<int>(modifier);
        return true;
    }
    return false;
}

void FaceModifier::setModifiers(unsigned int modifiers_) {
    modifiers = modifiers_;
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

FaceModifier::modifier FaceModifier::getMainModifier() const {
    return (modifier) (modifiers & ~(modifiers - 1));
}

int FaceModifier::getNumberOfModifiers() const {
    // `count` stores the total bits set in modifiers
    int count = 0;
    int modifiers_ = (int) modifiers;
    while (modifiers_) {
        modifiers_ = modifiers_ & (modifiers_ - 1);    // clear the least significant bit set
        count++;
    }

    return count;
}

}
