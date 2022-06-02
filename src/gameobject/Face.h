//
// Created by thijs on 01-06-22.
//

#ifndef DICEGONEROGUE_FACE_H
#define DICEGONEROGUE_FACE_H


#include <string>
#include <map>
#include <glm-0.9.7.1/glm/vec2.hpp>
#include <SpriteRenderer.h>

namespace DGR {

class Dice;

struct FaceModifier {
public:

    enum class modifier : unsigned int {
        ranged = 1,               // damage enemies in back row
        sweeping_edge = 2,        // damage the enemies left and right of the target
        single_use = 4,           // can only be used a single time
        poison = 8,               // debuff - keeps damaging the target every turn
        cleanse = 16,             // clears debuffs
        first_blood = 32,         // deals 2x damage on full hp targets
        dodge = 64,               // take no damage this turn
    };

    unsigned int modifiers;

    FaceModifier() : modifiers(0) {}

    explicit FaceModifier(unsigned int modifiers) : modifiers(modifiers) {};

    bool hasModifier(modifier mod);
};

enum faceType : int {
    damage,
    mana,
    heal,
    shield
};

class Face {
private:
    std::string name;
    Dice* dice{};

    int face_{};
    int value{};
    faceType type{};
    FaceModifier modifiers{};

    bool hover = false;

    static const std::map<int, const glm::vec2> faceDeltaPos;
    static const std::map<int, const glm::vec2> tickValueDeltaPos;

    static std::string faceTypeToString(faceType type);

    static glm::vec3 faceModifierToColor(FaceModifier modifier);

public:
    Face() = default;

    Face(std::string name, Dice* dice, int face_,
         int value = 0, faceType type = faceType::damage, FaceModifier modifiers = {});

    glm::vec2 getPosition() const;

    glm::vec2 getSize() const;

    void setValue(int value_);

    void setType(faceType type);

    void addModifier(FaceModifier::modifier modifier);

    void draw(SpriteRenderer* spriteRenderer);

    void setHover(bool hover_);

    bool isMouseHovering(double xPos, double yPos) const;

    void drawFace(SpriteRenderer* spriteRenderer);

    void drawFaceToolTip(SpriteRenderer* spriteRenderer);
};

}

#endif //DICEGONEROGUE_FACE_H
