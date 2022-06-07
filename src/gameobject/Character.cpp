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

void Character::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    spriteRenderer->drawSprite(name, 1.0f, position, size);

    dice->draw(spriteRenderer, textRenderer);
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

bool Character::interact(Character* otherCharacter) {
    // single character interactions
    if (!otherCharacter) {
        auto face = getDice()->getCurrentFace();
        FaceType type = face->getType();
        FaceModifier modifiers = face->getModifiers();

        switch (type.getType()) {
            case FaceType::empty:
                return true;
            case FaceType::mana:
                return true;
            case FaceType::dodge:
                return true;
            default:
                break;
        }

        return false;
    }


    bool differentCharacterType = (getCharacterType() != otherCharacter->getCharacterType());

    auto face = otherCharacter->getDice()->getCurrentFace();
    FaceType type = face->getType();
    FaceModifier modifiers = face->getModifiers();
    int value = face->getValue();

    switch (type.getType()) {
        case FaceType::damage:
            if (differentCharacterType) {
                hp -= value;
                return true;
            }
        case FaceType::heal:
            break;
        case FaceType::shield:
            if (!differentCharacterType) {
                shield += value;
                return true;
            }
        case FaceType::undying:
            break;
        case FaceType::heal_and_shield:
            break;
        case FaceType::heal_and_mana:
            break;
        case FaceType::shield_and_mana:
            break;
        case FaceType::damage_and_mana:
            break;
        case FaceType::damage_and_self_shield:
            break;
        default:
            break;
    }


    return false;
}

void Character::roll() {
    if (!dice->isLocked()) {
        dice->roll();
    }
}

void Character::setDiceLock(bool diceLock_) {
    dice->setLocked(diceLock_);
}

void Character::toggleDiceLock() {
    dice->setLocked(!dice->isLocked());
}

void Character::drawBox(SpriteRenderer* spriteRenderer, glm::vec3 color) {
    spriteRenderer->drawSprite("box", 0.4f, position, size, 1.0f, color, 0.0f);
}

bool Character::isMouseHovering(double xPos, double yPos, bool alsoCurrentFace) const {
    bool heroHover = (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);

    bool currentFaceHover = false;
    if (alsoCurrentFace) {
        currentFaceHover = dice->isMouseHovering(xPos, yPos, Dice::currentFacePos);
    }
    return heroHover || currentFaceHover;
}

void Character::setUsedDice(bool usedDice) {
    dice->setUsed(usedDice);
}

bool Character::getUsedDice() const {
    return dice->isUsed();
}

}