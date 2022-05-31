//
// Created by thijs on 31-05-22.
//

#include "Hero.h"

namespace DGR {

Hero::Hero(const std::string &textureName, glm::vec2 position, glm::vec2 size)
      : GameObject(textureName, position, size) {


}

void Hero::draw(SpriteRenderer* spriteRenderer) {
    spriteRenderer->drawSprite(textureName, position, size);

    glm::vec2 diceTemplatePos(position.x - 32, position.y + 40);
    glm::vec2 diceTemplateSize(size.x * 2, size.y*3/2);

    spriteRenderer->drawSprite("diceTemplate", diceTemplatePos, diceTemplateSize);

}

}