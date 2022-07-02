//
// Created by thijs on 19-06-22.
//

#include "SettingsScene.h"
#include <GameStateManager.h>
#include "ui/Button.h"

namespace DGR {

SettingsScene::SettingsScene(std::shared_ptr<GameStateManager> gameState)
      : Scene("SettingsScene", gameState,
              glm::vec2(DGR_WIDTH * 0.1, DGR_HEIGHT * 0.1),
              glm::vec2(DGR_WIDTH * 0.8, DGR_HEIGHT * 0.8)) {

    double width = size.x;
    double height = size.y;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;

    auto button1 = std::make_shared<Button>("Sound?",
                                            glm::vec2(width / 2 - buttonWidth / 2, i++ * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Sound?");
    buttons.push_back(button1);

    auto button2 = std::make_shared<Button>("HUDSize?",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("HUDSize?");
    buttons.push_back(button2);
}

void SettingsScene::handleMouseButton(double xPos, double yPos) {
    if (!isMouseHovering(xPos, yPos)) {
        gameState->popSceneFromStack();
    }

    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
        }
    }
}

void SettingsScene::pressButton(std::shared_ptr<Button> button) {
    std::cout << "pressed a button!" << std::endl;

}

void SettingsScene::render(std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<TextRenderer> textRenderer) {
    spriteRenderer->drawSprite("box", 1.0f, glm::vec2(0), size,
                               0.0f, glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
