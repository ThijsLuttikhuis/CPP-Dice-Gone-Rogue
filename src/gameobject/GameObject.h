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
    std::string textureName;

    glm::vec2 position;
    glm::vec2 size;

public:
    GameObject(std::string textureName, glm::vec2 position, glm::vec2 size);

    virtual void draw(SpriteRenderer* spriteRenderer);
};

}

#endif //DICEGONEROGUE_GAMEOBJECT_H
