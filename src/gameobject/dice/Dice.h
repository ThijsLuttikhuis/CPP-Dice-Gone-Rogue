//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_DICE_H
#define DICEGONEROGUE_DICE_H


#include <string>
#include <utility>
#include "shaders/SpriteRenderer.h"
#include "FaceModifier.h"
#include "FaceType.h"

namespace DGR {

class Character;
class Face;
class Dice {
public:
    enum dicePos {
        backgroundPos,
        diceLayoutPos,
        currentFacePos
    };

private:
    std::string name;

    Character* character = nullptr;
    Face* faces[6]{};

    int currentFace = 0;

public:
    Dice(std::string name, Character* character);

    Dice() = default;

    [[nodiscard]] Face* getFace(int index);

    [[nodiscard]] glm::vec2 getPosition(dicePos dicePos = backgroundPos) const;

    [[nodiscard]] glm::vec2 getSize(dicePos dicePos = backgroundPos) const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] const std::string &getName();

    void setFace(Face* face, int index);

    void setCharacter(Character* hero_);

    void setName(const std::string &name_);

    void updateHoverMouse(double xPos, double yPos);

    void drawHover(SpriteRenderer* spriteRenderer);

    void draw(SpriteRenderer* spriteRenderer);

    void roll();
};

}


#endif //DICEGONEROGUE_DICE_H
