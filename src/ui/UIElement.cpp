//
// Created by thijs on 13-06-22.
//

#include "UIElement.h"

namespace DGR {

void UIElement::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer) const {
    (void) textRenderer;

    spriteRenderer->drawSprite("box", 1.0f, position, size,
                               1.0f, glm::vec3(0.4), 0.0f);
}

void UIElement::setColor(const glm::vec3 &color_) {
    color = color_;
}

}