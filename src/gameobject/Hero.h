//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_HERO_H
#define DICEGONEROGUE_HERO_H

#include "GameObject.h"

namespace DGR {

class Dice;

class Hero : public GameObject {
private:
    Dice* dice;

public:
    Hero(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    void draw(SpriteRenderer* spriteRenderer) override;

    void drawHover(SpriteRenderer* spriteRenderer);

    Dice* getDice() const;
};

}

#endif //DICEGONEROGUE_HERO_H
