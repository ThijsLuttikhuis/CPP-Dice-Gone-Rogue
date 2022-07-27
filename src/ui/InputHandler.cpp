//
// Created by thijs on 30-05-22.
//

#include "ui/Button.h"
#include "InputHandler.h"

#include <utility>

namespace DGR {

InputHandler::InputHandler(std::weak_ptr<GameStateManager> gameState) : gameState(std::move(gameState)) {}

void InputHandler::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                          const std::unique_ptr<TextRenderer> &textRenderer) const {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

const Button &InputHandler::getButton(const std::string &name) {
    for (auto &button : buttons) {
        if (button->getName() == name) {
            return *button;
        }
    }
    return *nullButton;
}

}
