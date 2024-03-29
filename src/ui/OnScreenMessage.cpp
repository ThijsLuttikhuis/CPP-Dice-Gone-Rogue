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

void OnScreenMessage::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer) const {

    auto color_ = color;
    auto alpha_ = alpha;
    spriteArgs args = {{"color",     &color_},
                       {"alpha",     &alpha_},
                       {"edgeAlpha", &alpha_}};

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f, position, size, args);
    textRenderer->drawText(text, 0.0f, position, size, color, alpha);
}

}