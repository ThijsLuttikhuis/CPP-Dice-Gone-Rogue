//
// Created by thijs on 30-05-22.
//


#include <utility>

#include "utilities/Utilities.h"
#include "GameObject.h"
#include "shaders/SpriteRenderer.h"

namespace DGR {

int GameObject::uniqueIDCounter = 0;

int GameObject::getUniqueID() const {
    return uniqueID;
}

const glm::vec2 &GameObject::getPosition() const {
    return position;
}

const glm::vec2 &GameObject::getSize() const {
    return size;
}

const std::string &GameObject::getName() const {
    return name;
}

bool GameObject::isMouseHovering(double xPos, double yPos) const {
    return Utilities::isPositionInBox(xPos, yPos, position, size);
}

bool GameObject::getHoverMouse() const {
    return hover;
}

void GameObject::hoverMouse(bool hover_) {
    hover = hover_;
}

void GameObject::setUniqueID(int uniqueID_) {
    uniqueID = uniqueID_;
}

void GameObject::setPosition(glm::vec2 position_) {
    position = position_;
}

void GameObject::setSize(glm::vec2 size_) {
    size = size_;
}

void GameObject::setPosition(int left, int up) {
    setPosition(glm::vec2(left, up));
}

void GameObject::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) const {
    (void)textRenderer;
    spriteRenderer->drawSprite(name, 1.0f, position, size);
}

}
