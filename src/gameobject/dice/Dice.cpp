//
// Created by thijs on 31-05-22.
//

#include <utility>
#include <iostream>

#include "Dice.h"
#include "Face.h"
#include "gameobject/Hero.h"
#include "utilities/Random.h"
#include "utilities/Constants.h"

namespace DGR {

Dice::Dice(std::string name, Character* character) :
      name(std::move(name)), character(character) {
}

const std::string &Dice::getName() const {
    return name;
}

Dice* Dice::copy() const {
    auto copy = new Dice();
    copy->setName(name);

    copy->setLocked(lock);
    copy->setUsed(used);
    copy->setCurrentFace(currentFace);
    copy->setCurrentFaceHover(hoverCurrentFace);

    Face* faceCopy;
    for (int i = 0; i < 6; i++) {
        faceCopy = faces[i]->copy();
        faceCopy->setDice(copy);
        copy->setFace(faceCopy, i);
    }

    return copy;
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
    return (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);
}

Face* Dice::getFace(int index) const {
    return faces[index];
}

bool Dice::isUsed() const {
    return used;
}

Face* Dice::getCurrentFace() const {
    return faces[currentFace];
}

void Dice::setLocked(bool lock_) {
    lock = lock_;
}

void Dice::setCurrentFace(bool currentFace_) {
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

void Dice::setFace(Face* face, int index) {
    faces[index] = face;
}

void Dice::setCurrentFaceHover(bool hoverCurrentFace_) {
    hoverCurrentFace = hoverCurrentFace_;
    faces[currentFace]->setHover(hoverCurrentFace_);
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

void Dice::roll() {
    std::cout << "rolling" << std::endl;
    int rng = Random::randInt(0, 5);
    currentFace = rng;
}

void Dice::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
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

void Dice::drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    glm::vec2 diceTemplateBackgroundPosition = getPosition(Dice::backgroundPos);
    glm::vec2 diceTemplateBackgroundSize = getSize(Dice::backgroundPos);
    spriteRenderer->drawSprite("box", 0.9f,
                               diceTemplateBackgroundPosition, diceTemplateBackgroundSize, 1.0f, glm::vec3(0.2f), 0.5f);

    glm::vec2 diceTemplatePosition = getPosition(Dice::diceLayoutPos);
    glm::vec2 diceTemplateSize = getSize(Dice::diceLayoutPos);
    spriteRenderer->drawSprite("dice_template", 0.1f,
                               diceTemplatePosition, diceTemplateSize);

    for (auto &face : faces) {
        face->drawHover(spriteRenderer, textRenderer);
    }
}

}
