//
// Created by thijs on 01-06-22.
//

#ifndef DICEGONEROGUE_FACE_H
#define DICEGONEROGUE_FACE_H


#include <string>
#include <map>
#include <utility>
#include <glm-0.9.7.1/glm/vec2.hpp>
#include <shaders/SpriteRenderer.h>

namespace DGR {

class Dice;

class FaceModifier {
private:
    unsigned int modifiers;

public:
    FaceModifier() : modifiers(0) {}

    explicit FaceModifier(unsigned int modifiers) : modifiers(modifiers) {};

    enum class modifier : unsigned int {
        none = 0,
        ranged = 1,               // damage enemies in back row
        sweeping_edge = 2,        // damage the enemies left and right of the target
        single_use = 4,           // can only be used a single time
        poison = 8,               // debuff - keeps damaging the target every turn
        cleanse = 16,             // clears debuffs
        first_blood = 32,         // deals 2x damage on full hp targets
    };

    static modifier stringToModifier(const std::string &modifierStr);

    glm::vec3 toColor();

    unsigned int getModifiers();

    bool hasModifier(modifier mod);

    void addModifier(modifier modifier_);

    void addModifier(const std::string &modifierStr);

};

enum faceType : int {
    damage,
    mana,
    heal,
    shield,
    dodge,
    undying,
    heal_and_shield,
    heal_and_mana,
    shield_and_mana,
    damage_and_mana,
    damage_and_self_shield,
    empty
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

    void drawFace(SpriteRenderer* spriteRenderer);

    void drawFaceToolTip(SpriteRenderer* spriteRenderer);

public:
    Face() = default;

    Face(int face_, int value, faceType type, FaceModifier modifiers = {})
          : face_(face_), value(value), type(type), modifiers(modifiers) {};

    Face(std::string name, Dice* dice, int face_,
         int value = 0, faceType type = faceType::damage, FaceModifier modifiers = {});

    [[nodiscard]] glm::vec2 getPosition() const;

    [[nodiscard]] glm::vec2 getSize() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] int getFace_();

    void setValue(int value_);

    void setType(faceType type);

    void setHover(bool hover_);

    void setName(const std::string &name_);

    void setDice(Dice* dice);

    void addModifier(FaceModifier::modifier modifier);

    void addModifier(const std::string &modifierStr);

    void draw(SpriteRenderer* spriteRenderer);

};

}

#endif //DICEGONEROGUE_FACE_H
