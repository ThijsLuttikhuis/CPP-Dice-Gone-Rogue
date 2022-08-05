//
// Created by thijs on 01-06-22.
//

#include <sstream>
#include <iostream>
#include <utility>
#include <utilities/Utilities.h>

#include "Face.h"
#include "Dice.h"
#include "utilities/Constants.h"

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


Face::Face(std::string name, std::weak_ptr<Dice> dice,
           int face_, int value, int bonusValuePerLevel, FaceType type, FaceModifier modifiers)
      : name(std::move(name)), dice(std::move(dice)), bonusValuePerLevel(bonusValuePerLevel), face_(face_),
        level1DefaultValue(value), value(value), type(type), modifiers(modifiers) {
}

std::shared_ptr<Face> Face::makeCopy() const {
    auto copy = std::make_shared<Face>(name, dice, face_, value, bonusValuePerLevel, type, modifiers);
    return copy;
}

int Face::getFace_() const {
    return face_;
}

int Face::getValue() const {
    return value;
}

FaceType Face::getFaceType() const {
    return type;
}

FaceModifier Face::getModifiers() const {
    return modifiers;
}

glm::vec2 Face::getPosition(Dice::dicePos dicePos) const {
    auto dicePtr = std::shared_ptr<Dice>(dice);
    glm::vec2 dicePosition = dicePtr->getPosition(dicePos);
    switch (dicePos) {
        case Dice::background_pos:
            std::cerr << "Face::getPosition: dicePos backgroundPos should not be used" << std::endl;
            return glm::vec2(0, 0);
        case Dice::dice_layout_pos:
            dicePosition += faceDeltaPos.at(face_);
            break;
        case Dice::current_face_pos:
            dicePosition += glm::vec2(-8,2);
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

bool Face::isMouseHovering(double xPos, double yPos) const {
    auto position = getPosition();
    auto size = getSize();
    return (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);
}

void Face::setName(const std::string &name_) {
    name = name_;
}

void Face::setType(FaceType::faceType type_) {
    type = type_;
}

void Face::setType(FaceType type_) {
    type = type_;
}

void Face::setValue(int value_) {
    level1DefaultValue = value = value_;
}

void Face::setHover(bool hover_) {
    hover = hover_;
}

void Face::setDice(const std::weak_ptr<Dice> &dice_) {
    dice = dice_;
}

void Face::addModifier(FaceModifier::modifier modifier) {
    modifiers.addModifier(modifier);
}

void Face::addModifier(const std::string &modifierStr) {
    modifiers.addModifier(modifierStr);
}

void Face::setModifiers(unsigned int modifiers_) {
    modifiers.setModifiers(modifiers_);
}

void Face::removeModifier(FaceModifier::modifier modifier) {
    modifiers.removeModifier(modifier);
}

void Face::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer) const {
    drawFace(spriteRenderer, getPosition(Dice::current_face_pos));

    glm::vec3 textureColor = modifiers.toColor() * 2.0f / 3.0f;

    spriteRenderer->drawSprite("dice_3d_current_face", 0.22f, getPosition(Dice::current_face_pos) - glm::vec2(1,5),
                               getSize() + glm::vec2(6,6));

    spriteRenderer->drawSprite("dice_3d_current_face", 0.22f, getPosition(Dice::current_face_pos) - glm::vec2(1,5),
                               getSize() + glm::vec2(6,6));

    spriteRenderer->drawSprite("box", 0.2f, getPosition(Dice::current_face_pos),
                               getSize() - glm::vec2(1,1), 0.3f, textureColor, 0.0f);

}

void Face::drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer, Dice::dicePos dicePos) const {

    auto position = getPosition(dicePos);

    drawFace(spriteRenderer, position);

    if (hover) {
        drawFaceToolTip(spriteRenderer, textRenderer, position);
    }
}

void Face::drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer, glm::vec2 position) const {


    drawFace(spriteRenderer, position);

    if (hover) {
        drawFaceToolTip(spriteRenderer, textRenderer, position);
    }
}

void Face::drawLevelUpComparison(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                                 const std::unique_ptr<TextRenderer> &textRenderer, glm::vec2 position) {


    drawFace(spriteRenderer, position);

    drawFaceToolTip(spriteRenderer, textRenderer, position);


    int tooltipWidth = 96;
    int arrowWidth = 16;
    position += glm::vec2(tooltipWidth + 5, -5);

    spriteRenderer->drawSprite("arrow", 0.0f,
                               position, glm::vec2(arrowWidth));

    position += glm::vec2(arrowWidth - 5, +5);

    levelUp();
    drawFaceToolTip(spriteRenderer, textRenderer, position);
    level -= 2;
    levelUp();
}

void Face::drawCurrentFace(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer, glm::vec2 position) const {

    (void) textRenderer;

    position -= glm::vec2(1.0f);
    auto size = getSize();

    spriteRenderer->drawSprite("box", 0.05f, position, size, 1.0f,
                               glm::vec3(1.0f), 0.0f);

}

void Face::drawFace(const std::unique_ptr<SpriteRenderer> &spriteRenderer, glm::vec2 position) const {

    int value_ = value < 0 ? 0 : value > 40 ? 40 : value;

    // draw tick values as roman numerals
    int tens = value_ / 10;
    int fives = (value_ - tens * 10) / 5;
    int ones = (value_ - tens * 10 - fives * 5);

    int h = 0;
    while (h < 12) {
        glm::vec2 tickValueSize;
        glm::vec2 tickValuePos;
        if (tens > 0) {
            tickValueSize = glm::vec2(2, 3);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 2);
            spriteRenderer->drawSprite("pixel", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{1.0f, 0.5f, 0.0f});
            tens--;
            h += 4;
            continue;
        }
        if (fives > 0) {
            tickValueSize = glm::vec2(2, 2);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 1);
            spriteRenderer->drawSprite("pixel", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.8f, 0.0f});
            fives--;
            h += 3;
            continue;
        }
        if (ones == 4 || ones == 9) {
            tickValueSize = glm::vec2(2, 1);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h);
            spriteRenderer->drawSprite("pixel", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.9f, 0.9f});
            h += 2;

            if (ones == 4) {
                tickValueSize = glm::vec2(2, 2);
                tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 1);
                spriteRenderer->drawSprite("pixel", 0.1f, tickValuePos, tickValueSize,
                                           0.0f, glm::vec3{0.9f, 0.8f, 0.0f});
                h += 3;

            } else {
                tickValueSize = glm::vec2(2, 3);
                tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 2);
                spriteRenderer->drawSprite("pixel", 0.1f, tickValuePos, tickValueSize,
                                           0.0f, glm::vec3{1.0f, 0.5f, 0.0f});
                h += 4;
            }
            ones = 0;
            continue;
        }
        if (ones > 0) {
            tickValueSize = glm::vec2(2, 1);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h);
            spriteRenderer->drawSprite("pixel", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.9f, 0.9f});
            ones--;
            h += 2;
            continue;
        }
        break;
    }

    std::string textureNameGeneric = "hero_" + type.toString();
    std::string textureNameSpecific = name + "_" + type.toString();

    Utilities::spaceToUnderscore(textureNameGeneric);
    Utilities::spaceToUnderscore(textureNameSpecific);

    glm::vec3 textureColor = modifiers.toColor();
    if (spriteRenderer->hasTexture(textureNameSpecific)) {
        spriteRenderer->drawSprite(textureNameSpecific, 0.1f,
                                   position, glm::vec2(11, 14), 0, textureColor);
    } else {
        spriteRenderer->drawSprite(textureNameGeneric, 0.1f,
                                   position, glm::vec2(11, 14), 0, textureColor);
    }
}

void Face::drawFaceToolTip(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer,
                           glm::vec2 position) const {

    int tooltipWidth = 96;
    glm::vec2 tooltipDPos(5, -5);
    glm::vec2 tooltipSize(tooltipWidth, 1);

    glm::vec2 backgroundSize = glm::vec2(tooltipWidth, 16);
    spriteRenderer->drawSprite("box", 0.0f,
                               position + tooltipDPos, backgroundSize, 0.0f, glm::vec3(0.1f), 0.9f);


    std::string toolTipString = getToolTipString();
    glm::vec3 color = modifiers.toColor();
    textRenderer->drawText(toolTipString, 0.0f, position + tooltipDPos, tooltipSize, color);

#if DGR_DEBUG
    std::cout << "                       face: " << face_ << " -- value: " << value << " -- type: "
              << type.toString() << " -- modifiers: " << modifiers.toString() << std::endl;
#endif
}

std::shared_ptr<Face> Face::getSharedFromThis() {
    return shared_from_this();
}

std::string Face::getToolTipString() const {
    std::ostringstream tooltipOSS;
    if (value >= 0 && type != FaceType::empty) {
        tooltipOSS << value << " ";
    }
    tooltipOSS << type.toString();

    auto modStr = modifiers.toString();
    if (!modStr.empty()) {
        tooltipOSS << " (^" << modStr << "^)";
    }
    std::string toolTipString = tooltipOSS.str();
    return toolTipString;
}

void Face::levelUp() {
    level++;
    value = level1DefaultValue + bonusValuePerLevel * (level - 1);
}

void Face::setBonusValuePerLevel(int bonus) {
    bonusValuePerLevel = bonus;
}

}
