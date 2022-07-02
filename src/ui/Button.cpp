//
// Created by thijs on 08-06-22.
//

#include "Button.h"

namespace DGR {

bool Button::isPressed(double xPos, double yPos) const {
    return enabled ? isMouseHovering(xPos, yPos) : false;
}

void Button::draw( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) const {
    if (drawButton) {
        spriteRenderer->drawSprite("box", 1.0f, position, size,
                                   1.0f, color, textHasAlpha ? 0.0f : alpha);
    }

    textRenderer->drawText(text, 0.0f, position, size, color, textHasAlpha ? alpha : 1.0f);
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

}
