//
// Created by thijs on 01-06-22.
//

#include <iostream>
#include "Face.h"
#include "gameobject/dice/Dice.h"
#include "utilities/Constants.h"
#include "FaceType.h"
#include <sstream>

namespace DGR {

const std::vector<glm::vec2> Face::faceDeltaPos = {{
                                                         glm::vec2(2, 17),
                                                         glm::vec2(17, 2),
                                                         glm::vec2(17, 17),
                                                         glm::vec2(17, 32),
                                                         glm::vec2(32, 17),
                                                         glm::vec2(47, 17)}};


const std::vector<glm::vec2> Face::tickValueDeltaPos = {{
                                                              glm::vec2(11, 12),
                                                              glm::vec2(11, 10),
                                                              glm::vec2(11, 8),
                                                              glm::vec2(11, 6),
                                                              glm::vec2(11, 4),
                                                              glm::vec2(11, 2)}};


bool checkBit(unsigned int value, unsigned int pos) {
    return value & (1u << (pos));
}

void Face::setName(const std::string &name_) {
    name = name_;
}


Face::Face(std::string name, Dice* dice, int face_, int value, FaceType type, FaceModifier modifiers)
      : name(std::move(name)), dice(dice), face_(face_), value(value), type(type), modifiers(modifiers) {
}

void Face::setType(FaceType type_) {
    type = type_;
}

void Face::drawFace(SpriteRenderer* spriteRenderer, Dice::dicePos dicePos) {
    auto position = getPosition(dicePos);

    int value_ = value < 0 ? 0 : value > 40 ? 40 : value;

    int tens = value_ / 10;
    int fives = (value_ - tens * 10) / 5;
    int ones = (value_ - tens * 10 - fives * 5);

    int h = 0;
    while (h < 12) {
        glm::vec2 tickValueSize;
        glm::vec2 tickValuePos;
        if (tens > 0) {
            tickValueSize = glm::vec2(3, 3);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 2);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{1.0f, 0.5f, 0.0f});
            tens--;
            h += 4;
            continue;
        }
        if (fives > 0) {
            tickValueSize = glm::vec2(3, 2);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 1);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.8f, 0.0f});
            fives--;
            h += 3;
            continue;
        }
        if (ones == 4 || ones == 9) {
            tickValueSize = glm::vec2(3, 1);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.9f, 0.9f});
            h += 2;

            if (ones == 4) {
                tickValueSize = glm::vec2(3, 2);
                tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 1);
                spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                           0.0f, glm::vec3{0.9f, 0.8f, 0.0f});
                h += 3;

            } else {
                tickValueSize = glm::vec2(3, 3);
                tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 2);
                spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                           0.0f, glm::vec3{1.0f, 0.5f, 0.0f});
                h += 4;
            }
            ones = 0;
            continue;
        }
        if (ones > 0) {
            tickValueSize = glm::vec2(3, 1);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.9f, 0.9f});
            ones--;
            h += 2;
            continue;
        }
        break;
    }

    //TODO: create sprites for every hero type
    std::string textureName = "knight_" + type.toString();
    glm::vec3 textureColor = modifiers.toColor();
    spriteRenderer->drawSprite(textureName, 0.1f,
                               position, glm::vec2(11, 14), 0, textureColor);
}

void Face::drawFaceToolTip(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer, Dice::dicePos dicePos) {
    auto position = getPosition(dicePos);

    int tooltipWidth = 96;
    glm::vec2 tooltipDPos(5, -5);
    glm::vec2 tooltipSize(tooltipWidth, 1);

    glm::vec2 backgroundSize = glm::vec2(tooltipWidth, 16);
    spriteRenderer->drawSprite("box", 0.0f,
                               position + tooltipDPos, backgroundSize, 0.0f, glm::vec3(0.1f), 0.9f);

    std::ostringstream tooltipOSS;
    if (value != 0) {
        tooltipOSS << value << " ";
    }
    tooltipOSS << type.toString();

    auto modStr = modifiers.toString();
    if (!modStr.empty()) {
        tooltipOSS << " (^" << modStr << "^)";
    }

    glm::vec3 color = modifiers.toColor();
    textRenderer->drawText(tooltipOSS.str(), 0.0f, position + tooltipDPos, tooltipSize,
                              TextRenderer::center, color);

    std::cout << "                       face: " << face_ << " -- value: " << value << " -- type: "
              << type.toString() << " -- modifiers: " << modifiers.toString() << std::endl;
}

void Face::setValue(int value_) {
    value = value_;
}

glm::vec2 Face::getPosition(Dice::dicePos dicePos) const {
    glm::vec2 dicePosition = dice->getPosition(dicePos);
    switch (dicePos) {
        case Dice::backgroundPos:
            std::cerr << "Face::getPosition: dicePos backgroundPos should not be used" << std::endl;
            return glm::vec2(0,0);
        case Dice::diceLayoutPos:
            dicePosition += faceDeltaPos.at(face_);
            break;
        case Dice::currentFacePos:
            break;
        default:
            std::cerr << "Face::getPosition: dicePos unknown: " << dicePos << std::endl;
            break;
    }
    return dicePosition;
}

glm::vec2 Face::getSize() const {
    return glm::vec2(15, 15);
}

void Face::setHover(bool hover_) {
    hover = hover_;
}

bool Face::isMouseHovering(double xPos, double yPos) const {
    auto position = getPosition();
    auto size = getSize();
    return (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);
}

void Face::setDice(Dice* dice_) {
    dice = dice_;
}

void Face::addModifier(FaceModifier::modifier modifier) {
    modifiers.addModifier(modifier);
}

void Face::addModifier(const std::string &modifierStr) {
    modifiers.addModifier(modifierStr);
}

int Face::getFace_() const {
    return face_;
}

int Face::getValue() const {
    return value;
}

FaceType Face::getType() const {
    return type;
}

FaceModifier Face::getModifiers() const {
    return modifiers;
}

void Face::draw(SpriteRenderer* spriteRenderer) {
    drawFace(spriteRenderer, Dice::currentFacePos);
}

void Face::drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer, Dice::dicePos dicePos) {
    drawFace(spriteRenderer, dicePos);

    if (hover) {
        drawFaceToolTip(spriteRenderer, textRenderer, dicePos);
    }
}

}