//
// Created by thijs on 19-07-22.
//

#include "LevelSelectScene.h"
#include <utilities/Random.h>
#include "CharacterSelectScene.h"
#include "ui/Button.h"
#include "GameStateManager.h"
#include "BattleScene.h"
#include <set>
#include <utility>

namespace DGR {

LevelSelectScene::LevelSelectScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("LevelSelectScene", std::move(gameState)) {

    double width = DGR_WIDTH;
    double height = DGR_HEIGHT;

    double buttonWidth = width * 0.2;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    float i = 8;

    auto button1 = std::make_shared<Button>("Text", glm::vec2(width / 2 - buttonWidth, buttonDistance / 2),
                                            glm::vec2(buttonWidth * 2, buttonHeight / 2));
    button1->setText("Select level");
    buttons.push_back(button1);

    auto button2 = std::make_shared<Button>("Ready",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("To Battle!");
    buttons.push_back(button2);

    auto button3 = std::make_shared<Button>("Inventory",
                                            glm::vec2(width / 2 - buttonWidth / 2 + buttonWidth * 1.1,
                                                      i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button3->setText("Inventory");
    buttons.push_back(button3);

    auto button4 = std::make_shared<Button>("Return",
                                             glm::vec2(width / 2 - buttonWidth / 2 - buttonWidth * 1.1,
                                                       i * buttonDistance),
                                             glm::vec2(buttonWidth, buttonHeight));
    button4->setText("Return to main menu");
    buttons.push_back(button4);

    float dSizeX = 12.0f;
    float dSizeY = 6.0f;

    float leftRightButtonWidth = 24;
    float leftRightButtonHeight = 164;
    float leftRightButtonY = 48;

    float leftButtonX = 63;
    float rightButtonX = 409;

    int nMidButtonX = levelsPerRow;
    float midButtonStartX = leftButtonX + leftRightButtonWidth;
    float midButtonEndX = rightButtonX;
    float midButtonSizeX = (midButtonEndX - midButtonStartX) / (float)nMidButtonX;

    int nMidButtonY = numberOfLevels / levelsPerRow;
    float midButtonStartY = leftRightButtonY;
    float midButtonEndY = leftRightButtonY + leftRightButtonHeight;
    float midButtonSizeY = (midButtonEndY - midButtonStartY) / (float)nMidButtonY;

    auto button5 = std::make_shared<Button>("ScrollLeft",
                                            glm::vec2(leftButtonX, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight),
                                            glm::vec3(0.4f),
                                            0.0f, true, false);
    button5->setText(" <<");
    buttons.push_back(button5);

    auto button6 = std::make_shared<Button>("ScrollRight",
                                            glm::vec2(rightButtonX, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight),
                                            glm::vec3(0.4f),
                                            0.0f, true, false);
    button6->setText(" >>");
    buttons.push_back(button6);

    for (int index = 0; index < numberOfLevels; index++) {
        std::string istr = std::to_string(index+1);

        int xi = index % levelsPerRow;
        int yi = index / levelsPerRow;
        float xPos = midButtonStartX + midButtonSizeX * (float)xi;
        float yPos = midButtonStartY + midButtonSizeY * (float)yi;

        auto button_ = std::make_shared<Button>("Level" + istr,
                                                glm::vec2(xPos + dSizeX/2, yPos + dSizeY/2),
                                                glm::vec2(midButtonSizeX - dSizeX, midButtonSizeY - dSizeY),
                                                glm::vec3(0.4f),
                                                0.0f, false, false);
        button_->setText(istr);
        buttons.push_back(button_);
    }
}

void LevelSelectScene::handleMouseButton(double xPos, double yPos) {
    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
            return;
        }
    }
}

void LevelSelectScene::handleMousePosition(double xPos, double yPos) {
    (void)xPos, (void)yPos;
}

void LevelSelectScene::update(double dt) {
    t += dt;

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    for (auto &button : buttons) {
        button->update(t);
    }
}

void LevelSelectScene::render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                                  const std::shared_ptr<TextRenderer> &textRenderer) {

    spriteRenderer->drawSprite("box", 1.0f, glm::vec2(0), size,
                               0.0f, glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

void LevelSelectScene::pressButton(std::shared_ptr<Button> button) {
    std::cout << "pressed a button!" << std::endl;

    auto &buttonName = button->getName();

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    if (buttonName == "ScrollRight") {

    } else if (buttonName == "ScrollLeft") {

    } else if (buttonName == "Inventory") {

    } else if (buttonName == "Return") {
        gameStatePtr->popSceneFromStack();
    } else if (buttonName == "Ready") {

    } else if (buttonName.substr(0, 5) == "Level") {
        char** temp = nullptr;
        auto level = (int)strtol(buttonName.substr(5, buttonName.length() - 5).c_str(), temp, 10);

        /// set / reset color
        auto previouslySelectedButton = getButton("Level" + std::to_string(selectedLevel));
        if (previouslySelectedButton) {
            previouslySelectedButton->setColor(glm::vec3(0.4));
        }
        button->setColor(glm::vec3(1.0f, 0.0f, 0.0f));

        selectedLevel = level;
    }
}

}
