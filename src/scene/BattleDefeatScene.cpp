//
// Created by thijs on 20-06-22.
//

#include "BattleDefeatScene.h"
#include <GameStateManager.h>
#include "ui/Button.h"

namespace DGR {

BattleDefeatScene::BattleDefeatScene(std::shared_ptr<GameStateManager> gameState)
      : Scene("BattleDefeatScene", gameState,
              glm::vec2(DGR_WIDTH * 0.1, DGR_HEIGHT * 0.1),
              glm::vec2(DGR_WIDTH * 0.8, DGR_HEIGHT * 0.8)) {

    double width = size.x;
    double height = size.y;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;


    auto button1 = std::make_shared<Button>("Defeat :(",
                                             glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                             glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Defeat :(");
    buttons.push_back(button1);
}

void BattleDefeatScene::handleMouseButton(double xPos, double yPos) {
    if (!isMouseHovering(xPos, yPos)) {
        while (gameState->popSceneFromStack());
    }

    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
        }
    }
}

void BattleDefeatScene::pressButton(std::shared_ptr<Button> button) {
    std::cout << "pressed a button!" << std::endl;
}

void
BattleDefeatScene::render(std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<TextRenderer> textRenderer) {
    spriteRenderer->drawSprite("box", 1.0f, glm::vec2(0), size,
                               0.0f, glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
