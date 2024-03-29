//
// Created by thijs on 12-07-22.
//

#include "AreYouSureScene.h"
#include <GameStateManager.h>

#include <utility>
#include "ui/Button.h"

namespace DGR {

AreYouSureScene::AreYouSureScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("AreYouSureScene", std::move(gameState),
              glm::vec2(DGR_WIDTH * 0.3, DGR_HEIGHT * 0.3),
              glm::vec2(DGR_WIDTH * 0.4, DGR_HEIGHT * 0.4), glm::vec3(0.5, 0.2, 0.2)) {

    double width = size.x;
    double height = size.y;

    double buttonWidth = width * 0.7;
    double buttonHeight = height * 0.2;
    double buttonDistance = height * 0.2;
    int i = 1;


    auto button1 = std::make_unique<Button>("Are you sure?",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Are you sure?");
    buttons.push_back(std::move(button1));

    i+=2;

    auto button2 = std::make_unique<Button>("Yes",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth/3, buttonHeight));
    button2->setText("Yes");
    button2->setKeyboardKey(GLFW_KEY_ENTER);
    buttons.push_back(std::move(button2));

    auto button3 = std::make_unique<Button>("No",
                                            glm::vec2(width / 2 + buttonWidth / 2 - buttonWidth / 3, i * buttonDistance),
                                            glm::vec2(buttonWidth/3, buttonHeight));
    button3->setText("I will think about it");
    button3->setKeyboardKey(GLFW_KEY_BACKSPACE);
    buttons.push_back(std::move(button3));

    auto button4 = makeDefaultButton("Close", glm::vec2(width - 16.0f, 8.0f), glm::vec2(8.0f));
    buttons.push_back(std::move(button4));
}


void AreYouSureScene::handleMouseButton(double xPos, double yPos) {
    if (!isMouseHovering(xPos, yPos)) {
        messageSource = "";
        messageData = "";
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        gameStatePtr->popSceneFromStack();
    }

    handleMouseButtonDefault(xPos, yPos);
}

void AreYouSureScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;

    if (pressDefaultButton(button)) {
        return;
    }

    auto &buttonName = button->getName();

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    if (buttonName == "Yes") {
        gameStatePtr->getScene(messageSource)->message("yes: " + messageData);
        gameStatePtr->popSceneFromStack();
    } else if (buttonName == "No") {
        gameStatePtr->getScene(messageSource)->message("no: " + messageData);
        gameStatePtr->popSceneFromStack();
    }
}

std::string AreYouSureScene::message(const std::string &data) {

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    size_t posColon = data.find(':', 0);
    messageSource = data.substr(0, posColon);

    if (!gameStatePtr->getScene(messageSource)) {
        std::cerr << "AreYouSureScene::message(): source scene not found! exiting..." << std::endl;
        exit(404);
    }
    messageData = data.substr(posColon+2, data.size() - (posColon+2));

    return data;
}

}
