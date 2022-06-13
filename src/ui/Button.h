//
// Created by thijs on 08-06-22.
//

#ifndef DICEGONEROGUE_BUTTON_H
#define DICEGONEROGUE_BUTTON_H

#include <string>
#include <glm/glm.hpp>

#include "shaders/SpriteRenderer.h"
#include "shaders/TextRenderer.h"
#include "UIElement.h"

namespace DGR {

class Button : public UIElement {
private:
    std::string text;

public:
    Button(std::string name, const glm::vec2 &pos, const glm::vec2 &size, const glm::vec3 &color = glm::vec3(0.0f))
          : UIElement(std::move(name), pos, size, color) {}

    [[nodiscard]] bool isPressed(double xPos, double yPos) const;

    void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) const override;

    void setText(std::string text_);
};

}

#endif //DICEGONEROGUE_BUTTON_H
