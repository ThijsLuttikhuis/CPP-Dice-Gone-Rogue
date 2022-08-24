//
// Created by thijs on 19-06-22.
//

#include "SettingsScene.h"
#include <GameStateManager.h>

#include <utility>
#include "ui/Button.h"

namespace DGR {

SettingsScene::SettingsScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("SettingsScene", std::move(gameState),
              glm::vec2(DGR_WIDTH * 0.1, DGR_HEIGHT * 0.1),
              glm::vec2(DGR_WIDTH * 0.8, DGR_HEIGHT * 0.8), glm::vec3(0.2f)) {

    double width = size.x;
    double height = size.y;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;

    auto button1 = std::make_unique<Button>("Sound?",
                                            glm::vec2(width / 2 - buttonWidth / 2, i++ * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Sound?");
    buttons.push_back(std::move(button1));

    auto button2 = std::make_unique<Button>("HUDSize?",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("HUDSize?");
    buttons.push_back(std::move(button2));

    auto button4 = std::make_unique<Button>("Quit",
                                            glm::vec2(width / 2 - buttonWidth / 2, 8 * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button4->setText("Quit");
    buttons.push_back(std::move(button4));


    auto button3 = makeDefaultButton("Close", glm::vec2(width - 16.0f, 8.0f), glm::vec2(8.0f));
    buttons.push_back(std::move(button3));

}

void SettingsScene::handleMouseButton(double xPos, double yPos) {
    if (!isMouseHovering(xPos, yPos)) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        gameStatePtr->popSceneFromStack();
    }

    handleMouseButtonDefault(xPos, yPos);

}

void SettingsScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;
    if (pressDefaultButton(button)) {
        return;
    }
    auto &buttonName = button->getName();

    if (buttonName == "Quit") {
        Window::closeWindow();
    }
}


}
