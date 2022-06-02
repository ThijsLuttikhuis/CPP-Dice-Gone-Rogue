//
// Created by thijs on 01-06-22.
//

#include <iostream>
#include "Face.h"
#include "Dice.h"

namespace DGR {


const std::map<int, const glm::vec2> Face::faceDeltaPos = std::map<int, const glm::vec2>{
      {0, glm::vec2(2, 17)},
      {1, glm::vec2(17, 2)},
      {2, glm::vec2(17, 17)},
      {3, glm::vec2(17, 32)},
      {4, glm::vec2(32, 17)},
      {5, glm::vec2(47, 17)}};

const std::map<int, const glm::vec2> Face::tickValueDeltaPos = std::map<int, const glm::vec2>{
      {0, glm::vec2(11, 12)},
      {1, glm::vec2(11, 10)},
      {2, glm::vec2(11, 8)},
      {3, glm::vec2(11, 6)},
      {4, glm::vec2(11, 4)},
      {5, glm::vec2(11, 2)}};

bool checkBit(unsigned int value, unsigned int pos) {
    return value & (1u << (pos));
}

bool FaceModifier::hasModifier(FaceModifier::modifier modifier) {
    return modifiers & static_cast<unsigned int>(modifier);
}

Face::Face(std::string name, Dice* dice, int face_, int value, faceType type, FaceModifier modifiers)
      : name(std::move(name)), dice(dice), face_(face_), value(value), type(type), modifiers(modifiers) {

}

glm::vec3 Face::faceModifierToColor(FaceModifier modifiers) {
    if (modifiers.modifiers == 0) {
        return glm::vec3(1.0f);
    }

    if (modifiers.modifiers & static_cast<unsigned int>(FaceModifier::modifier::ranged)) {
        return glm::vec3(1.0f, 0.4f, 0.4f);
    }
    if (modifiers.modifiers & static_cast<unsigned int>(FaceModifier::modifier::sweeping_edge)) {
        return glm::vec3(4.0f, 4.5f, 1.0f);
    }
    if (modifiers.modifiers & static_cast<unsigned int>(FaceModifier::modifier::single_use)) {
        return glm::vec3(0.4f, 0.1f, 0.7f);
    }
    if (modifiers.modifiers & static_cast<unsigned int>(FaceModifier::modifier::poison)) {
        return glm::vec3(0.1f, 0.5f, 0.1f);
    }
    if (modifiers.modifiers & static_cast<unsigned int>(FaceModifier::modifier::cleanse)) {
        return glm::vec3(0.8f, 1.0f, 0.7f);
    }
    if (modifiers.modifiers & static_cast<unsigned int>(FaceModifier::modifier::first_blood)) {
        return glm::vec3(0.0f, 0.9f, 0.7f);
    }

    return glm::vec3(1.0f);
}


std::string Face::faceTypeToString(faceType type) {
    switch (type) {
        case damage:
            return "damage";
        case mana:
            return "mana";
        case heal:
            return "heal";
        case shield:
            return "shield";
        default:
            return "error";
    }
}

void Face::setType(faceType type_) {
    type = type_;
}

void Face::drawFace(SpriteRenderer* spriteRenderer) {
    auto position = getPosition();

    int value_ = value < 0 ? 0 : value > 63 ? 63 : value;

    for (unsigned int bit = 0; bit < 6; bit++) {
        const glm::vec2 tickValuePos = position + tickValueDeltaPos.at(bit);
        glm::vec2 tickValueSize(3, 1);
        if (checkBit(value_, bit)) {
            spriteRenderer->drawSprite("dice_face_on", tickValuePos, tickValueSize, 0, glm::vec3{1.0f}, 0.1f);
        } else {
            spriteRenderer->drawSprite("dice_face_off", tickValuePos, tickValueSize, 0, glm::vec3{1.0f}, 0.1f);
        }
    }

    //TODO: create sprites for every hero type
    std::string textureName = "knight_" + faceTypeToString(type);
    glm::vec3 textureColor = faceModifierToColor(modifiers);
    spriteRenderer->drawSprite(textureName, position, glm::vec2(11, 14), 0, textureColor, 0.1f);
}

void Face::drawFaceToolTip(SpriteRenderer* spriteRenderer) {
    auto position = getPosition();

    glm::vec2 backgroundSize = glm::vec2(32,16);
    spriteRenderer->drawSprite("dice_face_template_background", position + glm::vec2{5,-5}, backgroundSize,
                               0, glm::vec3{1.0f}, 0.0f);

    std::string textureName = "knight_" + faceTypeToString(type);
    glm::vec3 textureColor = faceModifierToColor(modifiers);
    spriteRenderer->drawSprite(textureName, position, glm::vec2(11, 14), 0, textureColor, 0.1f);
}

void Face::draw(SpriteRenderer* spriteRenderer) {

    drawFace(spriteRenderer);

    if (hover) {
        drawFaceToolTip(spriteRenderer);
    }
}

void Face::setValue(int value_) {
    value = value_;
}

void Face::addModifier(FaceModifier::modifier modifier_) {
    if (!modifiers.hasModifier(modifier_)) {
        modifiers.modifiers += static_cast<unsigned int>(modifier_);
    }
}

glm::vec2 Face::getPosition() const {
    glm::vec2 dicePosition = dice->getPosition(false);

    return dicePosition + faceDeltaPos.at(face_);
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



}