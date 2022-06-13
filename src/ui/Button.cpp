//
// Created by thijs on 08-06-22.
//

#include "Button.h"

namespace DGR {

bool Button::isPressed(double xPos, double yPos) const {
    return isMouseHovering(xPos, yPos);
}

void Button::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) const {
    spriteRenderer->drawSprite("box", 1.0f, position, size,
                               1.0f, glm::vec3(0.4), 0.0f);

    textRenderer->drawText(text, 0.0f, position, size, color, 1.0f);
}


void Button::setText(std::string text_) {
    text = std::move(text_);
}

}
