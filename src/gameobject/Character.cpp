//
// Created by thijs on 04-06-22.
//

#include "Character.h"

namespace DGR {

Character::Character(const std::string &name, glm::vec2 position, glm::vec2 size)
      : GameObject(name, position, size), dice(new Dice(name, this)) {
}

void Character::draw(SpriteRenderer* spriteRenderer) {
    spriteRenderer->drawSprite(name, 1.0f, position, size);
}

void Character::drawHover(SpriteRenderer* spriteRenderer) {
    if (hover) {
#if DEBUG
        std::cout << "hover: " << getName() << " -- x: " << getPosition().x
                  << " -- y: " << getPosition().y << std::endl;
#endif
        dice->draw(spriteRenderer);
    }
}

Dice* Character::getDice() const {
    return dice;
}

void Character::setDice(Dice* dice_) {
    dice = dice_;
}

void Character::setMaxHP(int maxHP_) {
    maxHP = maxHP_;
}

}