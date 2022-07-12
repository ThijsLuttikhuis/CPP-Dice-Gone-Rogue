//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_INPUTHANDLER_H
#define DICEGONEROGUE_INPUTHANDLER_H


#include <iostream>
#include <utility>
#include <vector>
#include <glm/vec2.hpp>

#include "shaders/SpriteRenderer.h"
#include "shaders/TextRenderer.h"

namespace DGR {

class Button;

class Character;

class InputHandler {
protected:
    GameStateManager* gameState;

    std::vector<Button*> buttons;
public:
    explicit InputHandler(GameStateManager* gameState);

    virtual void handleMouseButton(double xPos, double yPos) {
        (void)xPos, (void)yPos;
    };

    virtual void handleMousePosition(double xPos, double yPos) {
        (void)xPos, (void)yPos;
    };

    virtual void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);
};

}


#endif //DICEGONEROGUE_INPUTHANDLER_H
