//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_HERO_H
#define DICEGONEROGUE_HERO_H

#include "GameObject.h"

namespace DGR {

class Hero : public GameObject {
private:


public:
    Hero(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    void draw(SpriteRenderer* spriteRenderer) override;
};

}

#endif //DICEGONEROGUE_HERO_H
