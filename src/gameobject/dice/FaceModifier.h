//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_FACEMODIFIER_H
#define DICEGONEROGUE_FACEMODIFIER_H


#include <string>
#include <map>
#include <glm/vec3.hpp>

#include "utilities/BiDirectionalMap.h"

namespace DGR {

class FaceModifier {
public:
    enum class modifier : unsigned int {
        none = 0,
        ranged = 1,               // damage enemies in back row
        sweeping_edge = 2,        // damage the enemies left and right of the target
        single_use = 4,           // can only be used a single time
        poison = 8,               // debuff - keeps damaging the target every turn
        cleanse = 16,             // clears debuffs
        first_blood = 32,         // deals 2x damage on full hp targets
        growth = 64,              // grows by 1 every time used
        decay = 128,              // reduces by 1 every time used
    };
private:
    static BiDirectionalMap<std::string, modifier> stringsAndModifiers;

    unsigned int modifiers;
public:
    FaceModifier() : modifiers(0) {}

    explicit FaceModifier(unsigned int modifiers) : modifiers(modifiers) {};

    static modifier stringToModifier(const std::string &modifierStr);

    [[nodiscard]] std::string toString() const;

    [[nodiscard]] glm::vec3 toColor() const;

    [[nodiscard]] unsigned int getModifiers() const;

    [[nodiscard]] bool hasModifier(modifier mod) const;

    bool removeModifier(modifier modifier);

    void setModifiers(unsigned int modifiers_);

    void addModifier(modifier modifier_);

    void addModifier(const std::string &modifierStr);

};



}

#endif //DICEGONEROGUE_FACEMODIFIER_H
