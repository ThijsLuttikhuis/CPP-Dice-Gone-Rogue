//
// Created by thijs on 08-06-22.
//

#include "Button.h"

namespace DGR {

const glm::vec2 &Button::getPos() const {
    return pos;
}

const glm::vec2 &Button::getSize() const {
    return size;
}

const std::string &Button::getName() const {
    return name;
}

bool Button::isPressed(double xPos, double yPos) const {
    return xPos > pos.x && xPos < pos.x + size.x
           && yPos > pos.y && yPos < pos.y + size.y;
}

void Button::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) const {
    spriteRenderer->drawSprite("box", 1.0f, pos, size, 1.0f, glm::vec3(0.4), 0.0f);

    textRenderer->drawText(text, 0.0f, pos, size, color, 1.0f);
}

void Button::setText(std::string text_) {
    text = std::move(text_);
}

}
