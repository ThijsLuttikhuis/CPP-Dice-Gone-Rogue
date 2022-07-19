//
// Created by thijs on 13-06-22.
//

#ifndef DICEGONEROGUE_UIELEMENT_H
#define DICEGONEROGUE_UIELEMENT_H


#include "gameobject/GameObject.h"

namespace DGR {

class GameStateManager;

class UIElement : public GameObject {
protected:
    glm::vec3 color;
    float alpha;
public:
    UIElement(std::string name, const glm::vec2 &pos, const glm::vec2 &size,
              const glm::vec3 &color = glm::vec3(0.0f), float alpha = 1.0f)
          : GameObject(std::move(name), pos, size), color(color), alpha(alpha) {}

    void draw(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
              const std::shared_ptr<TextRenderer> &textRenderer) const override;

    void setColor(const glm::vec3 &color_);
};

}


#endif //DICEGONEROGUE_UIELEMENT_H
