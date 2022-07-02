//
// Created by thijs on 13-06-22.
//

#include "UIElement.h"

namespace DGR {

void UIElement::draw( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) const {
    (void) textRenderer;

    spriteRenderer->drawSprite("box", 1.0f, position, size,
                               1.0f, glm::vec3(0.4), 0.0f);
}

}