//
// Created by thijs on 31-05-22.
//

#include "Hero.h"
#include "Dice.h"

namespace DGR {

Hero::Hero(const std::string &name, glm::vec2 position, glm::vec2 size)
      : GameObject(name, position, size), dice(new Dice(name, this)) {
}

void Hero::draw(SpriteRenderer* spriteRenderer) {

    spriteRenderer->drawSprite(name, position, size);

}

void Hero::drawHover(SpriteRenderer* spriteRenderer) {
    if (hover) {
        dice->draw(spriteRenderer);
    }
}

Dice* Hero::getDice() const {
    return dice;
}

}