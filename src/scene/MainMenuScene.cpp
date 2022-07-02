//
// Created by thijs on 18-06-22.
//

#include <GameStateManager.h>
#include "MainMenuScene.h"
#include "ui/Button.h"

namespace DGR {

MainMenuScene::MainMenuScene(std::shared_ptr<GameStateManager> gameState) : Scene("MainMenuScene", gameState) {
    double width = DGR_WIDTH;
    double height = DGR_HEIGHT;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;

    auto button1 = std::make_shared<Button>("StartNewGame",
                                            glm::vec2(width / 2 - buttonWidth / 2, i++ * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Start New Game");
    buttons.push_back(button1);

    auto button2 = std::make_shared<Button>("LoadGame",
                                            glm::vec2(width / 2 - buttonWidth / 2, i++ * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("Load Game");
    buttons.push_back(button2);

    auto button3 = std::make_shared<Button>("Settings",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));

    button3->setText("Settings");
    buttons.push_back(button3);
}

void MainMenuScene::handleMouseButton(double xPos, double yPos) {
    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
        }
    }
}

void MainMenuScene::pressButton(std::shared_ptr<Button> button) {
    std::cout << "pressed a button!" << std::endl;

    if (button->getName() == "StartNewGame") {
        gameState->popSceneFromStack();
        gameState->addSceneToStack("CharacterSelectScene");
    } else if (button->getName() == "LoadGame") {
        gameState->addSceneToStack("LoadGameScene", false);
    } else if (button->getName() == "Settings") {
        gameState->addSceneToStack("SettingsScene");
    }
}

void
MainMenuScene::render(std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<TextRenderer> textRenderer) {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
