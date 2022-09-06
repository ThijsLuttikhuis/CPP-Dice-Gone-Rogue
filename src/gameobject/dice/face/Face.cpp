//
// Created by thijs on 01-06-22.
//

#include <iostream>
#include <utility>
#include <utilities/Utilities.h>

#include "Face.h"
#include "gameobject/dice/Dice.h"
#include "utilities/Constants.h"
#include "scene/BattleScene.h"

namespace DGR {

const std::vector<glm::vec2> Face::faceDeltaPos = {{
                                                         glm::vec2(2, 17),
                                                         glm::vec2(17, 2),
                                                         glm::vec2(17, 17),
                                                         glm::vec2(17, 32),
                                                         glm::vec2(32, 17),
                                                         glm::vec2(47, 17)}};


Face::Face(std::string name, std::weak_ptr<Dice> dice,
           int face_, int value, int bonusValuePerLevel, FaceModifier modifiers)
      : name(std::move(name)), dice(std::move(dice)), bonusValuePerLevel(bonusValuePerLevel), face_(face_),
        level1DefaultValue(value), value(value), modifiers(modifiers) {
}

int Face::getFace_() const {
    return face_;
}

int Face::getValue() const {
    return value;
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
            dicePosition += glm::vec2(-8, 2);
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

void Face::setModifiers(FaceModifier modifiers_) {
    modifiers = modifiers_;
}

void Face::removeModifier(FaceModifier::modifier modifier) {
    modifiers.removeModifier(modifier);
}

void Face::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer) const {

    drawFace(spriteRenderer, getPosition(Dice::current_face_pos));

    glm::vec3 textureColor = modifiers.toColor() * 2.0f / 3.0f;

    float alpha = 0.0f;
    float edgeAlpha = 0.3f;

    spriteArgs args = {{"color",     &textureColor},
                       {"alpha",     &alpha},
                       {"edgeAlpha", &edgeAlpha}};

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.2f,
                               getPosition(Dice::current_face_pos), getSize() - glm::vec2(1, 1), args);

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

    if (!bonusValuePerLevel) {
        return;
    }

    int tooltipWidth = 96;
    int arrowWidth = 16;
    position += glm::vec2(tooltipWidth + 5, -5);

    spriteRenderer->drawSprite("arrow", 0.0f,
                               position, glm::vec2(arrowWidth));

    position += glm::vec2(arrowWidth - 5, 5);

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

    auto color = glm::vec3(1.0f);
    float alpha = 0.0f;
    float edgeAlpha = 1.0f;

    spriteArgs args = {{"color",     &color},
                       {"alpha",     &alpha},
                       {"edgeAlpha", &edgeAlpha}};

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.05f, position, size, args);
}

void Face::drawFace(const std::unique_ptr<SpriteRenderer> &spriteRenderer, glm::vec2 position,
                    float rotate, float skewX, float skewY, float height, float width) const {

    auto originPosition = position;
    auto originSize = glm::vec2(14, 14);
    float alpha = 1.0f;
    auto color = glm::vec3(1.0f);
    spriteArgs args = {{"rotate",         &rotate},
                       {"skewx",          &skewX},
                       {"skewy",          &skewY},
                       {"originposition", &originPosition},
                       {"originsize",     &originSize},
                       {"height",         &height},
                       {"alpha",          &alpha},
                       {"color",          &color},
                       {"width",          &width}};

    int value_ = value < 0 ? 0 : value > 40 ? 40 : value;

    // draw tick values as roman numerals
    bool IV = false;
    bool IX = false;
    if (value_ % 10 == 9) {
        IX = true;
        value_ -= 9;
    } else if (value_ % 10 == 4) {
        IV = true;
        value_ -= 4;
    }

    int tens = value_ / 10;
    int fives = (value_ - tens * 10) / 5;
    int ones = (value_ - tens * 10 - fives * 5);

    int h = 0;
    int maxH = 12;
    while (h < maxH) {
        auto tickValueDeltaPos = glm::vec2(11, 12);

        glm::vec2 tickValueSize;
        glm::vec2 tickValuePos;
        if (tens > 0 && h < maxH - 3) {
            tickValueSize = glm::vec2(2, 3);
            tickValuePos = position + tickValueDeltaPos + glm::vec2(0, -h - 2);
            color = glm::vec3(1.0f, 0.5f, 0.0f);
            spriteRenderer->drawSprite(SpriteRenderer::shadow, "pixel", 0.1f,
                                       tickValuePos, tickValueSize, args);
            tens--;
            h += 4;
            continue;
        }
        if (fives > 0 && h < maxH - 2) {
            tickValueSize = glm::vec2(2, 2);
            tickValuePos = position + tickValueDeltaPos + glm::vec2(0, -h - 1);

            color = glm::vec3(0.9f, 0.8f, 0.0f);
            spriteRenderer->drawSprite(SpriteRenderer::shadow, "pixel", 0.1f,
                                       tickValuePos, tickValueSize, args);
            fives--;
            h += 3;
            continue;
        }
        if ((IV && maxH - 3) || (IX && h < maxH - 4)) {
            tickValueSize = glm::vec2(2, 1);
            tickValuePos = position + tickValueDeltaPos + glm::vec2(0, -h);

            color = glm::vec3(1.0f);
            spriteRenderer->drawSprite(SpriteRenderer::shadow, "pixel", 0.1f,
                                       tickValuePos, tickValueSize, args);
            h += 2;

            if (IV) {
                tickValueSize = glm::vec2(2, 2);
                tickValuePos = position + tickValueDeltaPos + glm::vec2(0, -h - 1);

                color = glm::vec3(0.9f, 0.8f, 0.0f);
                spriteRenderer->drawSprite(SpriteRenderer::shadow, "pixel", 0.1f,
                                           tickValuePos, tickValueSize, args);
                h += 3;
                IV = false;
            }
            if (IX) {
                tickValueSize = glm::vec2(2, 3);
                tickValuePos = position + tickValueDeltaPos + glm::vec2(0, -h - 2);

                color = glm::vec3(1.0f, 0.5f, 0.0f);
                spriteRenderer->drawSprite(SpriteRenderer::shadow, "pixel", 0.1f,
                                           tickValuePos, tickValueSize, args);
                h += 4;
                IX = false;
            }
            continue;
        }
        if (ones > 0 && h < maxH - 1) {
            tickValueSize = glm::vec2(2, 1);
            tickValuePos = position + tickValueDeltaPos + glm::vec2(0, -h);

            color = glm::vec3(0.9f, 0.9f, 0.9f);
            spriteRenderer->drawSprite(SpriteRenderer::shadow, "pixel", 0.1f,
                                       tickValuePos, tickValueSize, args);
            ones--;
            h += 2;
            continue;
        }
        break;
    }

    std::string textureNameGeneric = "hero_" + toString();
    std::string textureNameSpecific = name + "_" + toString();

    Utilities::spaceToUnderscore(textureNameGeneric);
    Utilities::spaceToUnderscore(textureNameSpecific);

    std::string textureName = spriteRenderer->hasTexture(textureNameSpecific) ? textureNameSpecific
                                                                              : textureNameGeneric;

    color = modifiers.toColor();
    spriteRenderer->drawSprite(SpriteRenderer::shadow, textureName, 0.1f,
                               position, glm::vec2(11, 14), args);

}

void Face::drawFaceToolTip(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer,
                           glm::vec2 position) const {

    int tooltipWidth = 96;
    glm::vec2 tooltipDPos(5, -5);
    glm::vec2 tooltipSize(tooltipWidth, 1);

    glm::vec2 backgroundSize = glm::vec2(tooltipWidth, 16);
    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.0f,
                               position + tooltipDPos, backgroundSize, glm::vec3(0.1f), 0.9f);


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
    tooltipOSS << value << " " << toString();

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

int Face::getBonusValuePerLevel() const {
    return bonusValuePerLevel;
}

std::string Face::toString() const {
    return "none";
}

bool Face::interactSelf(std::shared_ptr<Character> character,
                        std::shared_ptr<BattleScene> battleScene) {
    (void) character, (void) battleScene;

    return false;
}

bool Face::interactFriendly(std::shared_ptr<Character> character,
                        std::shared_ptr<BattleScene> battleScene) {
    (void) character, (void) battleScene;

    return false;
}

bool Face::interactFoe(std::shared_ptr<Character> character,
                        std::shared_ptr<BattleScene> battleScene) {
    (void) character, (void) battleScene;

    return false;
}


void Face::applySweepingEdge(const std::shared_ptr<Character>& character, const std::shared_ptr<Face> &face,
                                              const std::shared_ptr<BattleScene> &battleScene, bool isFoe) {

    face->removeModifier(FaceModifier::modifier::sweeping_edge);
    auto neighbours = battleScene->getNeighbours(character);
    for (auto &neighbour : {neighbours.first, neighbours.second}) {
        if (neighbour) {
            if (isFoe) {
                face->interactFoe(neighbour, battleScene);
            }
            else {
                face->interactFriendly(neighbour, battleScene);
            }
        }
    }
    face->addModifier(FaceModifier::modifier::sweeping_edge);
}


}
