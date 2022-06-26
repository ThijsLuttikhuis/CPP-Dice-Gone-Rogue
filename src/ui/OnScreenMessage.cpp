//
// Created by thijs on 26-06-22.
//

#include "OnScreenMessage.h"

#include <utility>

namespace DGR {


void OnScreenMessage::setText(std::string text_) {
    text = std::move(text_);
}

void OnScreenMessage::setFade(float fade_) {
    fadeSpeed = fade_;
}

void OnScreenMessage::setDuration(double duration_) {
    duration = duration_;
}

double OnScreenMessage::getDuration() const {
    return duration;
}

void OnScreenMessage::update(double dt) {
    duration -= dt;
    position = glm::vec2(position.x, position.y + moveSpeed * dt);
    alpha = alpha * (1.0f - (float) (dt / fadeSpeed));
}

void OnScreenMessage::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) const {
    spriteRenderer->drawSprite("box", 1.0f, position, size, alpha, color, alpha);
    textRenderer->drawText(text, 0.0f, position, size, color, alpha);
}

}