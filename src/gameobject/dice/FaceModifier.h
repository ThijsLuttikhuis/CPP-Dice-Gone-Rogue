//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_FACEMODIFIER_H
#define DICEGONEROGUE_FACEMODIFIER_H


#include <string>
#include <glm/vec3.hpp>

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
    unsigned int modifiers;
public:
    FaceModifier() : modifiers(0) {}

    explicit FaceModifier(unsigned int modifiers) : modifiers(modifiers) {};

    static modifier stringToModifier(const std::string &modifierStr);

    glm::vec3 toColor();

    unsigned int getModifiers();

    bool hasModifier(modifier mod);

    void addModifier(modifier modifier_);

    void addModifier(const std::string &modifierStr);

};

}

#endif //DICEGONEROGUE_FACEMODIFIER_H
