//
// Created by thijs on 30-05-22.
//



#include "GameObject.h"

#include <utility>
#include "SpriteRenderer.h"

namespace DGR {

GameObject::GameObject(std::string textureName, glm::vec2 position, glm::vec2 size)
      : textureName(std::move(textureName)), position(position), size(size) {

}

void GameObject::draw(SpriteRenderer* spriteRenderer) {
    spriteRenderer->drawSprite(textureName, position, size);
}

}
