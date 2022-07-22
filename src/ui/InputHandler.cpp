//
// Created by thijs on 30-05-22.
//

#include "ui/Button.h"
#include "InputHandler.h"

#include <utility>

namespace DGR {

InputHandler::InputHandler(std::weak_ptr<GameStateManager> gameState) : gameState(std::move(gameState)) {}

void InputHandler::render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                          const std::shared_ptr<TextRenderer> &textRenderer) const {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

std::shared_ptr<Button> InputHandler::getButton(const std::string &name) {
    for (auto &button : buttons) {
        if (button->getName() == name) {
            return button;
        }
    }
    return nullptr;
}

}
