//
// Created by thijs on 31-05-22.
//

#include "Dice.h"
#include "Hero.h"
#include <utility>
#include <iostream>
#include "utilities/Constants.h"

namespace DGR {


Dice::Dice(std::string name, Hero* hero) :
      name(std::move(name)), hero(hero),
      faces{Face(name, this, 0), Face(name, this, 1), Face(name, this, 2),
            Face(name, this, 3), Face(name, this, 4), Face(name, this, 5)} {

    faces[0].setValue(6);
    faces[0].setType(faceType::damage);
    faces[0].addModifier(FaceModifier::modifier::ranged);

    faces[1].setValue(9);
    faces[1].setType(faceType::damage);
    faces[1].addModifier(FaceModifier::modifier::sweeping_edge);

    faces[2].setValue(13);
    faces[2].setType(faceType::shield);
    faces[2].addModifier(FaceModifier::modifier::single_use);

    faces[3].setValue(19);
    faces[3].setType(faceType::damage);
    faces[3].addModifier(FaceModifier::modifier::poison);

    faces[4].setValue(21);
    faces[4].setType(faceType::damage);
    faces[4].addModifier(FaceModifier::modifier::cleanse);

    faces[5].setValue(22);
    faces[5].setType(faceType::shield);

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
#if DEBUG
    std::cout << "x: " << hero->getPosition().x << std::endl;
    std::cout << "x: " << hero->getName() << std::endl;
#endif

    glm::vec2 heroPosition = hero->getPosition();
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

void Dice::setHero(Hero* hero_) {
    hero = hero_;
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