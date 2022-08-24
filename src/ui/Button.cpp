//
// Created by thijs on 08-06-22.
//

#include "Button.h"

#include "utilities/Utilities.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace DGR {

bool Button::isPressed(double xPos, double yPos) const {
    return enabled ? isMouseHovering(xPos, yPos) : false;
}

void Button::drawKey(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer) const {
    (void) spriteRenderer;

    if (keyboardKey >= 0) {
        std::string keyName = Utilities::keyPressToName(keyboardKey);
        glm::vec2 keyPressSize = glm::vec2(keyName.length() * 4 + 2, 8);

        keyName = "^" + keyName + "^";
        textRenderer->drawText(keyName, 0.0f, position + size - keyPressSize, keyPressSize, color);
    }
}

void Button::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                  const std::unique_ptr<TextRenderer> &textRenderer) const {
    if (drawButton) {
        float alpha_ = textHasAlpha ? 0.0f : alpha;
        float edgeAlpha = 1.0f;
        auto color_ = color;

        spriteArgs args = {{"color",     &color_},
                           {"alpha",     &alpha_},
                           {"edgeAlpha", &edgeAlpha}};

        spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f, position, size, args);
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
        auto alpha_ = textHasAlpha ? 0.0f : alpha;
        auto edgeAlpha = 1.0f;
        spriteArgs args = {{"color",     &color_},
                           {"alpha",     &alpha_},
                           {"edgeAlpha", &edgeAlpha}};

        spriteRenderer->drawSprite(SpriteRenderer::box, "", zIndex, position, size, args);
    }

    textRenderer->drawText(text, 0.0f, position, size, color_, textHasAlpha ? alpha : 1.0f);
}

void Button::setText(std::string text_) {
    text = std::move(text_);
}

void Button::setKeyboardKey(int key) {
    keyboardKey = key;
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

bool Button::isKeyPressed(int key) const {
    return key == keyboardKey;
}

}
