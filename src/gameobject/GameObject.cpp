//
// Created by thijs on 30-05-22.
//



#include "GameObject.h"

#include <utility>
#include "shaders/SpriteRenderer.h"

namespace DGR {

GameObject::GameObject(std::string name, glm::vec2 position, glm::vec2 size)
      : name(std::move(name)), position(position), size(size) {
}

void GameObject::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    spriteRenderer->drawSprite(name, 1.0f, position, size);
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

void GameObject::hoverMouse(bool hover_) {
    hover = hover_;
}

bool GameObject::getHoverMouse() const {
    return hover;
}

bool GameObject::isMouseHovering(double xPos, double yPos) const {
    return (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);
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

}
