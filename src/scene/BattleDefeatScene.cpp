//
// Created by thijs on 20-06-22.
//

#include "BattleDefeatScene.h"
#include <GameStateManager.h>

#include <utility>
#include "ui/Button.h"

namespace DGR {

BattleDefeatScene::BattleDefeatScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("BattleDefeatScene", std::move(gameState),
              glm::vec2(DGR_WIDTH * 0.1, DGR_HEIGHT * 0.1),
              glm::vec2(DGR_WIDTH * 0.8, DGR_HEIGHT * 0.8)) {

    double width = size.x;
    double height = size.y;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;


    auto button1 = std::make_unique<Button>("Defeat :(",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Defeat :(");
    buttons.push_back(std::move(button1));
}

void BattleDefeatScene::handleMouseButton(double xPos, double yPos) {
    if (!isMouseHovering(xPos, yPos)) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        while (gameStatePtr->popSceneFromStack());
    }

    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
        }
    }
}

void BattleDefeatScene::pressButton(const std::unique_ptr<Button> &button) {
    (void) button;
    std::cout << "pressed a button!" << std::endl;
}

void BattleDefeatScene::onPopFromStack() {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
    gameStatePtr->getInventory()->reset();
    gameStatePtr->getGameProgress()->reset();
}

void BattleDefeatScene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                               const std::unique_ptr<TextRenderer> &textRenderer) const {
    spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f, glm::vec2(0), size,
                               glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
