//
// Created by thijs on 04-06-22.
//

#include "Character.h"
#include <iostream>
#include "dice/Face.h"
namespace DGR {

Character::Character(const std::string &name, glm::vec2 position, glm::vec2 size)
      : GameObject(name, position, size), dice(new Dice(name, this)) {
}

void Character::draw(SpriteRenderer* spriteRenderer) {
    spriteRenderer->drawSprite(name, 1.0f, position, size);

    dice->draw(spriteRenderer);
}

void Character::drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    if (hover) {
#if DEBUG
        std::cout << "hover: " << getName() << " -- x: " << getPosition().x
                  << " -- y: " << getPosition().y << std::endl;
#endif
        dice->drawHover(spriteRenderer, textRenderer);
    }
}

Dice* Character::getDice() const {
    return dice;
}

void Character::setDice(Dice* dice_) {
    dice = dice_;
}

void Character::setMaxHP(int maxHP_, bool setHPToMaxHP) {
    maxHP = maxHP_;
    if (setHPToMaxHP) {
        hp = maxHP_;
    }
}

bool Character::isDead() const {
    return hp <= 0;
}

void Character::dealDamage(Face* face) {
    FaceType type = face->getType();
    FaceModifier modifiers = face->getModifiers();
    int value = face->getValue();

    hp -= value;

    if (isDead()) {
        std::cout << name << " DED :(" << std::endl;
    }
}

void Character::roll() {
    dice->roll();
}

}