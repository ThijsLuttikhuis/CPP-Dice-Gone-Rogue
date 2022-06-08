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
#include <shaders/TextRenderer.h>

namespace DGR {

class GameController;

class Button;

class Character;

class InputHandler {
private:
    GameController* gameController;
    SpriteRenderer* spriteRenderer;
    TextRenderer* textRenderer;

    std::vector<Button*> buttons;
public:
    explicit InputHandler(GameController* gameController);

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void handleMousePosition(Character* character, double xPos, double yPos);

    void render();

    std::vector<Button*> getButtons();
};

}

#endif //DICEGONEROGUE_INPUTHANDLER_H
