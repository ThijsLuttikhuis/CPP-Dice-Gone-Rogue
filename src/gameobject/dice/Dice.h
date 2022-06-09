//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_DICE_H
#define DICEGONEROGUE_DICE_H


#include <string>
#include <utility>

#include "shaders/TextRenderer.h"
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
    bool lock = false;
    bool used = false;

    int currentFace = 0;
    bool hoverCurrentFace = false;

public:
    Dice(std::string name, Character* character);

    Dice() = default;

    [[nodiscard]] Face* getCurrentFace() const;

    [[nodiscard]] Face* getFace(int index) const;

    [[nodiscard]] glm::vec2 getPosition(dicePos dicePos = backgroundPos) const;

    [[nodiscard]] glm::vec2 getSize(dicePos dicePos = backgroundPos) const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, dicePos dicePos = backgroundPos) const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] bool isLocked() const;

    [[nodiscard]] bool isUsed() const;

    void setLocked(bool lock_);

    void setUsed(bool lock_);

    void setCurrentFace(bool currentFace_);

    void setCurrentFaceHover(bool hoverCurrentFace_);

    void setFace(Face* face, int index);

    void setCharacter(Character* hero_);

    void setName(const std::string &name_);

    void updateHoverMouse(double xPos, double yPos);

    void drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    void roll();


    Dice* copy();
};

}


#endif //DICEGONEROGUE_DICE_H
