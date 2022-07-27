//
// Created by thijs on 18-06-22.
//

#include <GameStateManager.h>

#include <utility>
#include "MainMenuScene.h"
#include "ui/Button.h"

namespace DGR {

MainMenuScene::MainMenuScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("MainMenuScene", std::move(gameState)) {
    double width = DGR_WIDTH;
    double height = DGR_HEIGHT;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;

    auto button1 = std::make_unique<Button>("StartNewGame",
                                            glm::vec2(width / 2 - buttonWidth / 2, i++ * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Start New Game");
    buttons.push_back(std::move(button1));

    auto button2 = std::make_unique<Button>("LoadGame",
                                            glm::vec2(width / 2 - buttonWidth / 2, i++ * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("Load Game");
    buttons.push_back(std::move(button2));

    auto button3 = std::make_unique<Button>("Settings",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));

    button3->setText("Settings");
    buttons.push_back(std::move(button3));
}

void MainMenuScene::handleMouseButton(double xPos, double yPos) {
    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
        }
    }
}

void MainMenuScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    if (button->getName() == "StartNewGame") {
        gameStatePtr->popSceneFromStack();
        gameStatePtr->pushSceneToStack("CharacterSelectScene");
    } else if (button->getName() == "LoadGame") {
        gameStatePtr->popSceneFromStack();
        gameStatePtr->pushSceneToStack("LoadGameScene", false);
    } else if (button->getName() == "Settings") {
        gameStatePtr->pushSceneToStack("SettingsScene");
    }
}

void MainMenuScene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer) const {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
