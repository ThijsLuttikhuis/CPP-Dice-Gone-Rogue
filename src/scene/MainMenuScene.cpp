//
// Created by thijs on 18-06-22.
//

#include <GameStateManager.h>
#include "MainMenuScene.h"
#include "ui/Button.h"

namespace DGR {


MainMenuScene::MainMenuScene(GameStateManager* gameState) : Scene("MainMenuScene", gameState) {
    double width = DGR_WIDTH;
    double height = DGR_HEIGHT;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;

    auto* button1 = new Button("StartNewGame", {width / 2 - buttonWidth / 2, i++ * buttonDistance},
                               {buttonWidth, buttonHeight});
    button1->setText("Start New Game");
    buttons.push_back(button1);

    auto* button2 = new Button("LoadGame", {width / 2 - buttonWidth / 2, i++ * buttonDistance},
                               {buttonWidth, buttonHeight});
    button2->setText("Load Game");
    buttons.push_back(button2);

    auto* button3 = new Button("Settings", {width / 2 - buttonWidth / 2, i * buttonDistance},
                               {buttonWidth, buttonHeight});
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

void MainMenuScene::handleMousePosition(double xPos, double yPos) {
    (void) xPos, (void) yPos;
}

void MainMenuScene::update(double dt) {
    (void) dt;
}

void MainMenuScene::pressButton(Button* button) {
    std::cout << "pressed a button!" << std::endl;

    if (button->getName() == "StartNewGame") {
        gameState->popSceneFromStack();
        gameState->addSceneToStack("BattleScene");
    }
    else if (button->getName() == "LoadGame") {
        gameState->addSceneToStack("LoadGameScene", false);
    }
    else if (button->getName() == "Settings") {
        gameState->addSceneToStack("SettingsScene");
    }
}

void MainMenuScene::render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

}
