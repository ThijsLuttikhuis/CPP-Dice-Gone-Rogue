//
// Created by thijs on 13-06-22.
//

#include "UIElement.h"

namespace DGR {

void UIElement::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer) const {
    (void) textRenderer;

    float edgeAlpha = 1.0f;
    auto color_ = color;
    auto alpha_ = alpha;
    spriteArgs args = {{"color",     &color_},
                       {"alpha",     &alpha_},
                       {"edgeAlpha", &edgeAlpha}};

    spriteRenderer->drawSprite("box", 1.0f, position, size, args);
}

void UIElement::setColor(const glm::vec3 &color_) {
    color = color_;
}

}