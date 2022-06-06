//
// Created by thijs on 31-05-22.
//

#include "Dice.h"
#include "gameobject/Hero.h"
#include <utility>
#include <iostream>
#include <utilities/Random.h>
#include "utilities/Constants.h"
#include "Face.h"

namespace DGR {


Dice::Dice(std::string name, Character* character) :
      name(std::move(name)), character(character) {
}

glm::vec2 Dice::getPosition(dicePos dicePos) const {
    glm::vec2 heroPosition = character->getPosition();
    glm::vec2 dPos(0, 0);
    switch (dicePos) {
        case backgroundPos:
            dPos = glm::vec2(-8, -63.99);
            break;
        case diceLayoutPos:
            dPos = glm::vec2(0, -56);
            break;
        case currentFacePos:
            dPos = glm::vec2(character->getSize().x / 2 - 8, character->getSize().y + 4);
            break;
        default:
            std::cerr << "Dice::getPosition: dicePos unknown: " << dicePos << std::endl;
            break;
    }
    return heroPosition + dPos;
}

glm::vec2 Dice::getSize(dicePos dicePos) const {
    switch (dicePos) {
        case backgroundPos:
            return glm::vec2(80, 64);
        case currentFacePos:
            return glm::vec2(64, 48);
        case diceLayoutPos:
            return glm::vec2(15, 15);
        default:
            std::cerr << "Dice::getSize: dicePos unknown: " << dicePos << std::endl;
            return glm::vec2(0, 0);

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
        face->setHover(face->isMouseHovering(xPos, yPos));
    }
}

void Dice::setFace(Face* face, int index) {
    faces[index] = face;
}

void Dice::setCharacter(Character* character_) {
    character = character_;
}

void Dice::setName(const std::string &name_) {
    name = name_;
    for (auto &face : faces) {
        face->setName(name_);
    }
}

Face* Dice::getFace(int index) {
    return faces[index];
}

const std::string &Dice::getName() {
    return name;
}

void Dice::draw(SpriteRenderer* spriteRenderer) {
    faces[currentFace]->draw(spriteRenderer);
}

void Dice::drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    glm::vec2 diceTemplateBackgroundPosition = getPosition();
    glm::vec2 diceTemplateBackgroundSize = getSize();
    spriteRenderer->drawSprite("dice_template_background", 0.9f,
                               diceTemplateBackgroundPosition, diceTemplateBackgroundSize);

    glm::vec2 diceTemplatePosition = getPosition(Dice::diceLayoutPos);
    glm::vec2 diceTemplateSize = getSize(Dice::currentFacePos);
    spriteRenderer->drawSprite("dice_template", 0.1f,
                               diceTemplatePosition, diceTemplateSize);

    for (auto &face : faces) {
        face->drawHover(spriteRenderer, textRenderer);
    }

}

void Dice::roll() {
    std::cout <<"rolling" << std::endl;
    int rng = Random::randInt(0, 5);
    currentFace = rng;
}

}