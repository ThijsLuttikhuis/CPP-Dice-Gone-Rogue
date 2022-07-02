//
// Created by thijs on 30-05-22.
//

#include "ui/Button.h"
#include "InputHandler.h"

namespace DGR {

InputHandler::InputHandler( std::shared_ptr<GameStateManager> gameState) : gameState(gameState) {}

void InputHandler::render( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
