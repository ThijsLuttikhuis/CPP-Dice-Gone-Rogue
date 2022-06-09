//
// Created by thijs on 30-05-22.
//


#include <utility>

#include "GameObject.h"
#include "shaders/SpriteRenderer.h"

namespace DGR {

GameObject::GameObject(std::string name, glm::vec2 position, glm::vec2 size)
      : name(std::move(name)), position(position), size(size) {
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
    return (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);
}

bool GameObject::getHoverMouse() const {
    return hover;
}

void GameObject::hoverMouse(bool hover_) {
    hover = hover_;
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

void GameObject::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    (void)textRenderer;
    spriteRenderer->drawSprite(name, 1.0f, position, size);
}

}
