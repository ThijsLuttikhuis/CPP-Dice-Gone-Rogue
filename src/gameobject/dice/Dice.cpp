//
// Created by thijs on 31-05-22.
//

#include "Dice.h"
#include "gameobject/Hero.h"
#include <utility>
#include <iostream>
#include "utilities/Constants.h"

namespace DGR {


Dice::Dice(std::string name, Character* character) :
      name(std::move(name)), character(character) {
}

void Dice::draw(SpriteRenderer* spriteRenderer) {
    glm::vec2 diceTemplateBackgroundPosition = getPosition();
    glm::vec2 diceTemplateBackgroundSize = getSize();
    spriteRenderer->drawSprite("dice_template_background", 0.9f,
                               diceTemplateBackgroundPosition, diceTemplateBackgroundSize);

    glm::vec2 diceTemplatePosition = getPosition(false);
    glm::vec2 diceTemplateSize = getSize(false);
    spriteRenderer->drawSprite("dice_template", 0.1f,
                               diceTemplatePosition, diceTemplateSize);

    for (auto &face : faces) {
        face.draw(spriteRenderer);
    }
}

glm::vec2 Dice::getPosition(bool backgroundPos) const {
    glm::vec2 heroPosition = character->getPosition();
    glm::vec2 diceTemplatePosition(heroPosition.x - 8, heroPosition.y - 63.99);
    if (!backgroundPos) {
        diceTemplatePosition += glm::vec2(8, 8);
    }
    return diceTemplatePosition;
}

glm::vec2 Dice::getSize(bool backgroundSize) const {
    if (backgroundSize) {
        return glm::vec2(80, 64);
    } else {
        return glm::vec2(64, 48);
    }
}

bool Dice::isMouseHovering(double xPos, double yPos) const {
    auto position = getPosition();
    auto size = getSize();
    return (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);
}

void Dice::updateHoverMouse(double xPos, double yPos) {
    for (auto &face : faces) {
        face.setHover(face.isMouseHovering(xPos, yPos));
    }
}

void Dice::setFace(Face face, int index) {
    faces[index] = std::move(face);
}

void Dice::setCharacter(Character* character_) {
    character = character_;
}

void Dice::setName(const std::string &name_) {
    name = name_;
    for (auto & face : faces) {
        face.setName(name_);
    }
}

Face* Dice::getFace(int index) {
    return &faces[index];
}

const std::string &Dice::getName() {
    return name;
}

}