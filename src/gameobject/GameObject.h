//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_GAMEOBJECT_H
#define DICEGONEROGUE_GAMEOBJECT_H

#include <glm/vec3.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"

namespace DGR {

class GameObject {
protected:
    std::string name;

    glm::vec2 position;
    glm::vec2 size;

    bool hover = false;
public:
    GameObject(std::string name, glm::vec2 position, glm::vec2 size);

    const glm::vec2 &getPosition() const;

    const glm::vec2 &getSize() const;

    const std::string &getName() const;

    void hoverMouse(bool hover);

    virtual void draw(SpriteRenderer* spriteRenderer);

    bool isMouseHovering(double xPos, double yPos) const;

    bool getHoverMouse() const;
};

}

#endif //DICEGONEROGUE_GAMEOBJECT_H
