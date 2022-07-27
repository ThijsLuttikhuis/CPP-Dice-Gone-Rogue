//
// Created by thijs on 08-06-22.
//

#include "Button.h"

#include <iostream>

namespace DGR {

bool Button::isPressed(double xPos, double yPos) const {
    return enabled ? isMouseHovering(xPos, yPos) : false;
}

void Button::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                  const std::unique_ptr<TextRenderer> &textRenderer) const {
    if (drawButton) {
        spriteRenderer->drawSprite("box", 1.0f, position, size,
                                   1.0f, color, textHasAlpha ? 0.0f : alpha);
    }

    float blinkAlpha = alpha;
    if (doBlink) {
        blinkAlpha = 0.5f * (float) (1.0f + std::sin(blinkTimer * 2.0 * 3.14159 / blinkPeriod));
    }
    textRenderer->drawText(text, 0.0f, position, size, color, textHasAlpha ? blinkAlpha : 1.0f);
}


void Button::drawColor(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                       const std::unique_ptr<TextRenderer> &textRenderer, glm::vec3 color_, float zIndex) const {
    if (drawButton) {
        spriteRenderer->drawSprite("box", zIndex, position, size,
                                   1.0f, color_, textHasAlpha ? 0.0f : alpha);
    }

    textRenderer->drawText(text, 0.0f, position, size, color_, textHasAlpha ? alpha : 1.0f);
}

void Button::setText(std::string text_) {
    text = std::move(text_);
}

bool Button::isEnabled() const {
    return enabled;
}

void Button::setEnabled(bool enabled_) {
    enabled = enabled_;
}

void Button::update(double t) {
    if (doBlink) {
        blinkTimer = t;
    }
}

void Button::setDoBlink(bool doBlink_) {
    doBlink = doBlink_;
}

}
