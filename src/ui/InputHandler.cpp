//
// Created by thijs on 30-05-22.
//

#include "ui/Button.h"
#include "InputHandler.h"

namespace DGR {

InputHandler::InputHandler(GameStateManager* gameState) : gameState(gameState) {}

void InputHandler::render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
