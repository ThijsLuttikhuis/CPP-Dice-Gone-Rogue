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

        poison = 4,                     // debuff - keeps damaging the target every turn
        regen = 8,                      // buff - keeps healing the target every turn

        cleanse = 1024 * 2,             // clears debuffs
        sweep = 1024 * 4,               // damage the enemies left and right of the target

        self_shield = 1024 * 32,        // shield yourself for the same amount
        mana_gain = 1024 * 64,          // also gain that much mana
        charged = 1024 * 128,           // gain bonus equal to mana

        growth = 1024 * 256,            // grows by 1 every time used
        decay = 1024 * 512,             // reduces by 1 every time used

        backstab = 1024 * 1024 * 4,     // deals 2x damage on full hp targets
        ranged = 1024 * 1024 * 8,       // damage enemies in back row


        instant = 1024 * 1024 * 512,    // applies instantly without delays
        single_use = 1024 * 1024 * 1024,// can only be used a single time

    };


private:
    static BiDirectionalMap<std::string, modifier> stringsAndModifiers;
    static std::map<FaceModifier::modifier, glm::vec3> modifierToColor;

    unsigned int modifiers;
public:
    FaceModifier() : modifiers(0) {}

    explicit FaceModifier(unsigned int modifiers) : modifiers(modifiers) {};

    explicit FaceModifier(modifier modifier_) : modifiers(static_cast<unsigned int>(modifier_)) {};

    static modifier stringToModifier(const std::string &modifierStr);

    /// getters
    [[nodiscard]] std::string toString() const;

    [[nodiscard]] glm::vec3 toColor() const;

    [[nodiscard]] unsigned int getModifiers() const;

    [[nodiscard]] bool hasModifier(modifier mod) const;

    [[nodiscard]] modifier getMainModifier() const;

    [[nodiscard]] int getNumberOfModifiers() const;

    /// setters
    bool removeModifier(modifier modifier);

    void setModifiers(unsigned int modifiers_);

    void addModifier(modifier modifier_);

    void addModifier(const std::string &modifierStr);

    std::vector<glm::vec3> toAllColors() const;

    std::vector<FaceModifier::modifier> getAllModifiers() const;

    std::string toAllModifiersString() const;
};


}

#endif //DICEGONEROGUE_FACEMODIFIER_H
