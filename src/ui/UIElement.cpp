//
// Created by thijs on 13-06-22.
//

#include "UIElement.h"

namespace DGR {

void UIElement::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) const {
    (void) textRenderer;

    spriteRenderer->drawSprite("box", 1.0f, position, size,
                               1.0f, glm::vec3(0.4), 0.0f);
}

}