//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_GAMEOBJECT_H
#define DICEGONEROGUE_GAMEOBJECT_H

#include <utility>
#include <glm/vec3.hpp>
#include <shaders/TextRenderer.h>
#include "shaders/Texture2D.h"
#include "shaders/SpriteRenderer.h"

namespace DGR {

class GameObject {
protected:
    std::string name;

    glm::vec2 position{};
    glm::vec2 size{};

    bool hover = false;
public:
    GameObject() = default;

    explicit GameObject(std::string name) : name(std::move(name)) {};

    GameObject(std::string name, glm::vec2 position, glm::vec2 size);

    [[nodiscard]] const glm::vec2 &getPosition() const;

    [[nodiscard]] const glm::vec2 &getSize() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] bool getHoverMouse() const;

    void setPosition(glm::vec2 position_);

    void setPosition(int left, int up);

    void setSize(glm::vec2 size);

    void hoverMouse(bool hover);

    virtual void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);
};

}

#endif //DICEGONEROGUE_GAMEOBJECT_H
