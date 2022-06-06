//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_INPUTHANDLER_H
#define DICEGONEROGUE_INPUTHANDLER_H

#include <utility>
#include <vector>
#include <glm/vec2.hpp>
#include "shaders/SpriteRenderer.h"
#include <iostream>

namespace DGR {

class GameController;

class Character;

class Button {
private:
    std::string name;

    glm::vec2 pos;
    glm::vec2 size;
public:
    Button(std::string name, const glm::vec2 &pos, const glm::vec2 &size) : name(std::move(name)), pos(pos), size(size) {}

    [[nodiscard]] const glm::vec2 &getPos() const {
        return pos;
    }

    [[nodiscard]] const glm::vec2 &getSize() const {
        return size;
    }

    [[nodiscard]] const std::string &getName() const {
        return name;
    }

    [[nodiscard]] bool isPressed(double xPos, double yPos) const {
        return xPos > pos.x && xPos < pos.x + size.x && yPos > pos.y && yPos < pos.y + size.y;
    }

    void draw(SpriteRenderer* spriteRenderer) {
        spriteRenderer->drawSprite(name, 0.1f, pos, size);
    }
};

class InputHandler {
private:
    GameController* gameController;
    SpriteRenderer* spriteRenderer;

    std::vector<Button*> buttons;
public:
    explicit InputHandler(GameController* gameController);

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void handleMousePosition(Character* character, double xPos, double yPos);

    void render();
};

}

#endif //DICEGONEROGUE_INPUTHANDLER_H
