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
    button1->setKeyboardKey(GLFW_KEY_1);
    buttons.push_back(std::move(button1));

    auto button2 = std::make_unique<Button>("LoadGame",
                                            glm::vec2(width / 2 - buttonWidth / 2, i++ * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("Load Game");
    button2->setKeyboardKey(GLFW_KEY_2);
    buttons.push_back(std::move(button2));

    auto button3 = Scene::makeDefaultButton("Settings",
                                                   glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                                   glm::vec2(buttonWidth, buttonHeight));

    buttons.push_back(std::move(button3));
}

void MainMenuScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;

    if (pressDefaultButton(button)) {
        return;
    }

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

}
