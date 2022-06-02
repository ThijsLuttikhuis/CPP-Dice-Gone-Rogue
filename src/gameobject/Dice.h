//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_DICE_H
#define DICEGONEROGUE_DICE_H


#include <string>
#include <utility>
#include "SpriteRenderer.h"
#include "Face.h"

namespace DGR {

class Hero;

class Dice {
private:
    std::string name;

    Hero* hero;
    Face faces[6];

public:
    Dice(std::string name, Hero* hero);

    glm::vec2 getPosition(bool backgroundPos = true) const;

    glm::vec2 getSize(bool backgroundSize = true) const;

    void draw(SpriteRenderer* spriteRenderer);

    bool isMouseHovering(double xPos, double yPos) const;

    void updateHoverMouse(double xPos, double yPos);
};

}


#endif //DICEGONEROGUE_DICE_H
