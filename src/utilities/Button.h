//
// Created by thijs on 08-06-22.
//

#ifndef DICEGONEROGUE_BUTTON_H
#define DICEGONEROGUE_BUTTON_H

#include <string>
#include <glm/glm.hpp>

#include "shaders/SpriteRenderer.h"
#include "shaders/TextRenderer.h"

namespace DGR {

class Button {
private:
    std::string name;

    glm::vec2 pos;
    glm::vec2 size;

    glm::vec3 color;
    std::string text;

public:
    Button(std::string name, const glm::vec2 &pos, const glm::vec2 &size, const glm::vec3 &color = glm::vec3(0.0f))
          : name(std::move(name)), pos(pos), size(size), color(color){}

    [[nodiscard]] const glm::vec2 &getPos() const;

    [[nodiscard]] const glm::vec2 &getSize() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] bool isPressed(double xPos, double yPos) const;

    void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) const;

    void setText(std::string text_);
};

}

#endif //DICEGONEROGUE_BUTTON_H
