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

    bool enabled = true;
    bool textHasAlpha = true;
    bool drawButton = true;

    bool doBlink = false;
    double blinkPeriod = 2.0;
    double blinkTimer = 0.0;

public:
    Button(std::string name, const glm::vec2 &pos, const glm::vec2 &size,
           const glm::vec3 &color = glm::vec3(0.4f), float alpha = 0.0f, bool textHasAlpha = false,
           bool doBlink = false, double blinkPeriod = 2.0f)
          : UIElement(std::move(name), pos, size, color, alpha),
            textHasAlpha(textHasAlpha), doBlink(doBlink), blinkPeriod(blinkPeriod) {}

    Button(std::string name, const glm::vec2 &pos, const glm::vec2 &size, bool drawButton)
          : UIElement(std::move(name), pos, size), drawButton(drawButton) {}

    [[nodiscard]] bool isPressed(double xPos, double yPos) const;

    [[nodiscard]] bool isEnabled() const;

    void setDoBlink(bool doBlink_);

    void setEnabled(bool enabled_);

    void draw(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
              const std::shared_ptr<TextRenderer> &textRenderer) const override;

    void update(double t);

    void setText(std::string text_);

    void drawColor(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                   const std::shared_ptr<TextRenderer> &textRenderer, glm::vec3 color_, float zIndex = 0.9f) const;
};

}

#endif //DICEGONEROGUE_BUTTON_H
