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
#include "Button.h"

namespace DGR {

class Character;

class InputHandler {
protected:
    std::weak_ptr<GameStateManager> gameState;

    std::vector<std::unique_ptr<Button>> buttons;
    std::unique_ptr<Button> nullButton = std::make_unique<Button>("", glm::vec2(), glm::vec2());

    bool drawKeyPress = false;

    virtual void pressButton(const std::unique_ptr<Button> &button) {
        (void) button;
    };

public:
    explicit InputHandler(std::weak_ptr<GameStateManager> gameState);

    const Button &getButton(const std::string &name);


    virtual void handleKeyboard(int key, int action, const std::unique_ptr<std::vector<bool>> &keysPressed) {
        (void) key, (void) action, (void) keysPressed;
    };

    virtual void handleMouseButton(double xPos, double yPos) {
        (void) xPos, (void) yPos;
    };

    virtual void handleMousePosition(double xPos, double yPos) {
        (void) xPos, (void) yPos;
    };

    virtual void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                        const std::unique_ptr<TextRenderer> &textRenderer) const;

};

}


#endif //DICEGONEROGUE_INPUTHANDLER_H
