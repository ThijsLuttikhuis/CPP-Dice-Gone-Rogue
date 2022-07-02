//
// Created by thijs on 31-05-22.
//

#include <memory>
#include <utility>
#include <iostream>
#include <utilities/Utilities.h>

#include "Dice.h"
#include "Face.h"
#include "utilities/Random.h"
#include "utilities/Constants.h"
#include "gameobject/Character.h"

namespace DGR {

Dice::Dice(std::string name,  std::shared_ptr<Character> character) :
      name(std::move(name)), character(character) {
}

const std::string &Dice::getName() const {
    return name;
}

 std::shared_ptr<Dice> Dice::makeCopy() const {
    auto copy = std::make_shared<Dice>();

     std::shared_ptr<Face> faceCopy;
    for (int i = 0; i < 6; i++) {
        faceCopy = faces[i]->makeCopy();
        faceCopy->setDice(copy);
        copy->setFace(faceCopy, i);
    }

    copy->setName(name);

    copy->setLocked(lock);
    copy->setUsed(used);
    copy->setCurrentFace(currentFace);
    copy->setCurrentFaceHover(hoverCurrentFace);

    copy->setCharacter(character);
    return copy;
}

glm::vec2 Dice::getPosition(dicePos dicePos) const {
    glm::vec2 heroPosition = character->getPosition();
    glm::vec2 dPos(0, 0);
    switch (dicePos) {
        case backgroundPos:
            dPos = glm::vec2(-8, -95.99 + std::max(character->getSize().y, 32.0f));
            break;
        case diceLayoutPos:
            dPos = glm::vec2(0, -84 + std::max(character->getSize().y, 32.0f));
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

bool Dice::isLocked() const {
    return lock;
}

glm::vec2 Dice::getSize(dicePos dicePos) const {
    switch (dicePos) {
        case backgroundPos:
            return glm::vec2(80, 64);
        case currentFacePos:
            return glm::vec2(15, 15);
        case diceLayoutPos:
            return glm::vec2(64, 48);
        default:
            std::cerr << "Dice::getSize: dicePos unknown: " << dicePos << std::endl;
            return glm::vec2(0, 0);
    }
}

bool Dice::isMouseHovering(double xPos, double yPos, dicePos dicePos) const {
    auto position = getPosition(dicePos);
    auto size = getSize(dicePos);
    return Utilities::isPositionInBox(xPos, yPos, position, size);
}

 std::shared_ptr<Face> Dice::getFace(int index) const {
    return faces[index];
}

 std::shared_ptr<Character> Dice::getCharacter() const {
    return character;
}

bool Dice::isUsed() const {
    return used;
}

 std::shared_ptr<Face> Dice::getCurrentFace() const {
    return faces[currentFace];
}

void Dice::setLocked(bool lock_) {
    lock = lock_;
}

void Dice::setCurrentFace(int currentFace_) {
    currentFace = currentFace_;
}

void Dice::setUsed(bool used_) {
    used = used_;
}

void Dice::updateHoverMouse(double xPos, double yPos) {
    for (auto &face : faces) {
        face->setHover(face->isMouseHovering(xPos, yPos));
    }
}

void Dice::setFace( std::shared_ptr<Face> face, int index) {
    faces[index] = face;
}

void Dice::setCurrentFaceHover(bool hoverCurrentFace_) {
    hoverCurrentFace = hoverCurrentFace_;
    faces[currentFace]->setHover(hoverCurrentFace_);
}

void Dice::setCharacter( std::shared_ptr<Character> character_) {
    character = character_;
}

void Dice::setName(const std::string &name_) {
    name = name_;
    for (auto &face : faces) {
        face->setName(name_);
    }
}

void Dice::roll() {
    int rng = Random::randInt(0, 5);
    currentFace = rng;
}

void Dice::draw( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) {
    faces[currentFace]->draw(spriteRenderer);

    if (lock) {
        glm::vec2 lockPosition = getPosition(Dice::currentFacePos) + glm::vec2(-2, -2);
        glm::vec2 lockSize = glm::vec2(11, 14);
        spriteRenderer->drawSprite("dice_lock", 0.05f,
                                   lockPosition, lockSize, 0.0f, glm::vec3(1.0), 0.8f);
    }

    if (used) {
        glm::vec2 usedPosition = getPosition(Dice::currentFacePos);
        glm::vec2 usedSize = getSize(Dice::currentFacePos);
        spriteRenderer->drawSprite("box", 0.05f,
                                   usedPosition, usedSize, 0.0f, glm::vec3(1.0), 0.3f);
    }

    if (hoverCurrentFace) {
        faces[currentFace]->drawHover(spriteRenderer, textRenderer, currentFacePos);
    }
}

void Dice::drawHover( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) {
    glm::vec2 diceTemplateBackgroundPosition = getPosition(Dice::backgroundPos);
    glm::vec2 diceTemplateBackgroundSize = getSize(Dice::backgroundPos);
    spriteRenderer->drawSprite("box", 0.9f, diceTemplateBackgroundPosition,
                               diceTemplateBackgroundSize, 1.0f, glm::vec3(0.2f), 0.5f);

    glm::vec2 diceTemplateTextBoxSize = glm::vec2(getSize(Dice::backgroundPos).x, 8);
    spriteRenderer->drawSprite("box", 0.8f, diceTemplateBackgroundPosition,
                               diceTemplateTextBoxSize, 1.0f, glm::vec3(0.2f), 0.5f);
    textRenderer->drawText(name, 0.1f, diceTemplateBackgroundPosition, diceTemplateTextBoxSize);

    glm::vec2 diceTemplatePosition = getPosition(Dice::diceLayoutPos);
    glm::vec2 diceTemplateSize = getSize(Dice::diceLayoutPos);
    spriteRenderer->drawSprite("dice_template", 0.1f,
                               diceTemplatePosition, diceTemplateSize);

    for (auto &face : faces) {
        face->drawHover(spriteRenderer, textRenderer);
    }
}

}
