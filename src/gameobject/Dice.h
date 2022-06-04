//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_DICE_H
#define DICEGONEROGUE_DICE_H


#include <string>
#include <utility>
#include "shaders/SpriteRenderer.h"
#include "Face.h"

namespace DGR {

class Hero;

class Dice {
private:
    std::string name;

    Hero* hero = nullptr;
    Face faces[6]{};

public:
    Dice(std::string name, Hero* hero);

    Dice() = default;

    [[nodiscard]] Face* getFace(int index);

    [[nodiscard]] glm::vec2 getPosition(bool backgroundPos = true) const;

    [[nodiscard]] glm::vec2 getSize(bool backgroundSize = true) const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] const std::string &getName();

    void setFace(Face face, int index);

    void setHero(Hero* hero_);

    void setName(const std::string &name_);

    void updateHoverMouse(double xPos, double yPos);

    void draw(SpriteRenderer* spriteRenderer);
};

}


#endif //DICEGONEROGUE_DICE_H
