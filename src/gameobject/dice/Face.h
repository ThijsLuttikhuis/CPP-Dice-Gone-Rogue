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
#include "FaceModifier.h"
#include "FaceType.h"

namespace DGR {

class Dice;


class Face {
private:
    std::string name;
    Dice* dice{};

    int face_{};
    int value{};
    FaceType type{};
    FaceModifier modifiers{};

    bool hover = false;

    static const std::map<int, const glm::vec2> faceDeltaPos;
    static const std::map<int, const glm::vec2> tickValueDeltaPos;


    void drawFace(SpriteRenderer* spriteRenderer);

    void drawFaceToolTip(SpriteRenderer* spriteRenderer);

public:
    Face() = default;

    Face(int face_, int value, FaceType type, FaceModifier modifiers = {})
          : face_(face_), value(value), type(type), modifiers(modifiers) {};

    Face(std::string name, Dice* dice, int face_,
         int value, FaceType type, FaceModifier modifiers);

    [[nodiscard]] glm::vec2 getPosition() const;

    [[nodiscard]] glm::vec2 getSize() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] int getFace_();

    void setValue(int value_);

    void setType(FaceType type);

    void setHover(bool hover_);

    void setName(const std::string &name_);

    void setDice(Dice* dice);

    void addModifier(FaceModifier::modifier modifier);

    void addModifier(const std::string &modifierStr);

    void draw(SpriteRenderer* spriteRenderer);

};

}

#endif //DICEGONEROGUE_FACE_H
