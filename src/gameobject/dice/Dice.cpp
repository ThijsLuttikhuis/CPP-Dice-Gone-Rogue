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

const std::string &Dice::getName() const {
    return name;
}

std::shared_ptr<Dice> Dice::makeCopy() const {
    auto copy = std::make_shared<Dice>();

    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Face> faceCopy = faces[i]->makeCopy();
        faceCopy->setDice(copy->getSharedFromThis());
        copy->setFace(faceCopy->getSharedFromThis(), i);
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
    auto characterPtr = std::shared_ptr<Character>(character);
    glm::vec2 heroPosition = characterPtr->getPosition();
    glm::vec2 dPos(0, 0);
    switch (dicePos) {
        case background_pos:
            dPos = glm::vec2(-8, -111.99 + std::max(characterPtr->getSize().y, 48.0f));
            break;
        case dice_layout_pos:
            dPos = glm::vec2(0, -100 + std::max(characterPtr->getSize().y, 48.0f));
            break;
        case current_face_pos:
            dPos = glm::vec2(characterPtr->getSize().x / 2 - 8, characterPtr->getSize().y + 4);
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
        case background_pos:
            return glm::vec2(80, 64);
        case current_face_pos:
            return glm::vec2(15, 15);
        case dice_layout_pos:
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

bool Dice::isUsed() const {
    return used;
}

std::shared_ptr<Face> Dice::getCurrentFace() const {
    if (currentFace < 0 || currentFace > 5) {
        return nullptr;
    }
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

void Dice::setFace(const std::shared_ptr<Face> &face_, int index) {
    faces[index] = face_;
}

void Dice::setCurrentFaceHover(bool hoverCurrentFace_) {
    hoverCurrentFace = hoverCurrentFace_;
    if (currentFace >= 0 && currentFace < 6) {
        faces[currentFace]->setHover(hoverCurrentFace_);
    }
}

void Dice::setCharacter(const std::weak_ptr<Character> &character_) {
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

void Dice::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) {

    // draw 3D dice + shadow
    glm::vec2 dicePosition = getPosition(current_face_pos) + glm::vec2(-9, -3);
    glm::vec2 diceSize = getSize(current_face_pos) + glm::vec2(6, 6);

    spriteArgs args = {};

    spriteRenderer->drawSprite(SpriteRenderer::shadow, "dice_3d_current_face", 0.22f,
                               dicePosition, diceSize, args);
    spriteRenderer->drawSprite("dice_3d_current_face", 0.21f, dicePosition, diceSize);

    // draw three visible faces of the dice

    // 0245 -> right = 2450; 13 -> right = (-90)4(90)4
    // 0245 -> up = (-90)1(0)1(90)1(180)1; 13 -> up = (180)5(0)2
    float upRotate[6] = {-90, 180, 0, 0, 90, 180};
    int upFace[6] = {1, 5, 1, 2, 1, 1};
    float rightRotate[6] = {0, -90, 0, 90, 0, 0};
    int rightFace[6] = {2, 4, 4, 4, 5, 0};

    faces[currentFace]->draw(spriteRenderer);

    float skew = 30.0f;

    faces[upFace[currentFace]]->drawFace(spriteRenderer, getPosition(Dice::current_face_pos) + glm::vec2(-10, -13.5),
                                         upRotate[currentFace], skew, 0.0f, 0.2f, 1.0f);

    faces[rightFace[currentFace]]->drawFace(spriteRenderer, getPosition(Dice::current_face_pos) + glm::vec2(-3, -3),
                                            rightRotate[currentFace], 0.0f, skew, 1.0f, 0.25f);

    // draw lock symbol if dice is locked
    if (lock) {
        glm::vec2 lockPosition = getPosition(Dice::current_face_pos) + glm::vec2(-12, -2);
        glm::vec2 lockSize = glm::vec2(11, 14);

        spriteRenderer->drawSprite(SpriteRenderer::default_sprite, "dice_lock",
                                   0.05f, lockPosition, lockSize);

        spriteRenderer->drawSprite("dice_lock", 0.05f, lockPosition, lockSize, glm::vec3(1.0), 0.8f);
    }

    // draw grey box if dice is used
    if (used) {
        glm::vec2 usedPosition = getPosition(Dice::current_face_pos) + glm::vec2(-8, 2);
        glm::vec2 usedSize = getSize(Dice::current_face_pos);
        spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.05f,
                                   usedPosition, usedSize, glm::vec3(1.0), 0.3f);
    }

    // draw reminder text if dice is hovered
    if (hoverCurrentFace) {
        faces[currentFace]->drawHover(spriteRenderer, textRenderer, current_face_pos);
    }
}

void Dice::drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer, bool drawCurrentFaceIndicator) const {

    auto characterPtr = std::shared_ptr<Character>(character);

    glm::vec2 diceTemplateBackgroundPosition = getPosition(Dice::background_pos);
    glm::vec2 diceTemplateBackgroundSize = getSize(Dice::background_pos);

    auto color = glm::vec3(0.2f);
    float alpha = 0.5f;
    float edgeAlpha = 1.0f;
    spriteArgs args = {{"color",     &color},
                       {"alpha",     &alpha},
                       {"edgeAlpha", &edgeAlpha}};

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.9f, diceTemplateBackgroundPosition,
                               diceTemplateBackgroundSize, args);

    glm::vec2 diceTemplateTextBoxSize = glm::vec2(getSize(Dice::background_pos).x, 8);
    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.8f, diceTemplateBackgroundPosition,
                               diceTemplateTextBoxSize, args);
    textRenderer->drawText(name + "  [Lvl " + std::to_string(characterPtr->getLevel()) + "]", 0.1f,
                           diceTemplateBackgroundPosition, diceTemplateTextBoxSize);

    glm::vec2 diceTemplatePosition = getPosition(Dice::dice_layout_pos);
    glm::vec2 diceTemplateSize = getSize(Dice::dice_layout_pos);
    spriteRenderer->drawSprite("dice_template", 0.1f,
                               diceTemplatePosition, diceTemplateSize);

    for (auto &face : faces) {
        face->drawHover(spriteRenderer, textRenderer);
    }
    if (drawCurrentFaceIndicator && currentFace >= 0 && currentFace < 6) {
        auto face = faces[currentFace];
        face->drawCurrentFace(spriteRenderer, textRenderer, face->getPosition(Dice::dice_layout_pos));
    }
}

std::shared_ptr<Dice> Dice::getSharedFromThis() {
    return shared_from_this();
}

}
