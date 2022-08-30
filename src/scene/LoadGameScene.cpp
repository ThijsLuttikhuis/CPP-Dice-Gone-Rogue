//
// Created by thijs on 10-07-22.
//

#include "LoadGameScene.h"
#include <utilities/Random.h>
#include "ui/Button.h"
#include "GameStateManager.h"
#include "BattleScene.h"
#include <set>
#include <utility>
#include <filesystem>
#include "io/BattleLog.h"

namespace DGR {

LoadGameScene::LoadGameScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("LoadGameScene", std::move(gameState)) {

    double width = DGR_WIDTH;
    double height = DGR_HEIGHT;

    double buttonWidth = width * 0.2;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    float i = 8;

    auto button1 = std::make_unique<Button>("Text", glm::vec2(width / 2 - buttonWidth, buttonDistance / 2),
                                            glm::vec2(buttonWidth * 2, buttonHeight / 2));
    button1->setText("Load game");
    buttons.push_back(std::move(button1));

    auto button2 = std::make_unique<Button>("Ready",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("Load save file");
    buttons.push_back(std::move(button2));

    auto button9 = std::make_unique<Button>("DeleteSave",
                                            glm::vec2(width / 2 - buttonWidth / 2 + buttonWidth * 1.1,
                                                      i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button9->setText("Delete save file");
    buttons.push_back(std::move(button9));

    auto button10 = std::make_unique<Button>("Return",
                                             glm::vec2(width / 2 - buttonWidth / 2 - buttonWidth * 1.1,
                                                       i * buttonDistance),
                                             glm::vec2(buttonWidth, buttonHeight));
    button10->setText("Return to main menu");
    buttons.push_back(std::move(button10));

    float leftRightButtonWidth = 24;
    float leftRightButtonHeight = 164;
    float leftRightButtonY = 48;

    float leftButtonX = 63;
    float rightButtonX = 409;

    float nMidButton = 4;
    float midButtonStart = leftButtonX + leftRightButtonWidth;
    float midButtonEnd = rightButtonX;
    float midButtonWidth = (midButtonEnd - midButtonStart) / nMidButton;
    i = 0;

    auto button3 = std::make_unique<Button>("ScrollLeft",
                                            glm::vec2(leftButtonX, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight),
                                            glm::vec3(0.4f),
                                            0.0f, true, true);
    button3->setText(" <<");
    buttons.push_back(std::move(button3));

    auto button4 = std::make_unique<Button>("ScrollRight",
                                            glm::vec2(rightButtonX, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight),
                                            glm::vec3(0.4f),
                                            0.0f, true, true);
    button4->setText(" >>");
    buttons.push_back(std::move(button4));


    auto button5 = std::make_unique<Button>("SelectSave0",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight));
    button5->setText("");
    buttons.push_back(std::move(button5));

    auto button6 = std::make_unique<Button>("SelectSave1",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight));
    button6->setText("");
    buttons.push_back(std::move(button6));

    auto button7 = std::make_unique<Button>("SelectSave2",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight));
    button7->setText("");
    buttons.push_back(std::move(button7));

    auto button8 = std::make_unique<Button>("SelectSave3",
                                            glm::vec2(midButtonStart + i * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight));
    button8->setText("");
    buttons.push_back(std::move(button8));

}

void LoadGameScene::handleMousePosition(const std::shared_ptr<Character> &character, double xPos, double yPos) {
    auto dice = character->getDice();
    dice->setCurrentFaceHover(dice->isMouseHovering(xPos, yPos, Dice::current_face_pos));

    if (character->isMouseHovering_(xPos, yPos)) {
        character->setHoverMouse(true);
    } else if (character->getHoverMouse()) {
        if (dice->isMouseHovering(xPos, yPos)) {
            dice->updateHoverMouse(xPos, yPos);
        } else {
            character->setHoverMouse(false);
        }
    }
}

void LoadGameScene::handleMouseButton(double xPos, double yPos) {

    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
            return;
        }
    }
}

void LoadGameScene::handleMousePosition(double xPos, double yPos) {
    (void) xPos, (void) yPos;
    //TODO: show preview of save game
}

void LoadGameScene::reset() {
    loadedGames = {};
    selectedGame = 0;

    maxSavesOnRow = 4;
    currentLeftSaveIndex = 0;
}

void LoadGameScene::update(double dt) {
    t += dt;

    for (auto &button : buttons) {
        if (button->getName() == "ScrollRight") {
            button->setDoBlink((int) loadedGameNames.size() > currentLeftSaveIndex + maxSavesOnRow);
        } else if (button->getName() == "ScrollLeft") {
            button->setDoBlink(currentLeftSaveIndex > 0);
        }

        button->update(t);
    }
}

void LoadGameScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;

    auto &buttonName = button->getName();

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    if (buttonName == "ScrollRight") {
        currentLeftSaveIndex++;
        currentLeftSaveIndex = std::min(currentLeftSaveIndex,
                                        std::max(0, (int) loadedGames.size() - maxSavesOnRow));
    } else if (buttonName == "ScrollLeft") {
        currentLeftSaveIndex--;
        currentLeftSaveIndex = std::max(currentLeftSaveIndex, 0);

    } else if (buttonName == "Return") {
        gameStatePtr->popSceneFromStack();

    } else if (buttonName == "DeleteSave") {
        if (selectedGame >= (int) loadedGames.size()) {
            gameStatePtr->addOnScreenMessage("No data found, please select another save.");
            return;
        }
        auto &fileName = loadedGameNames[selectedGame];
        std::string dir = "../src/io/saves/";
        auto dirIt = std::filesystem::directory_iterator(dir);
        for (const auto &entry : dirIt) {
#if DGR_DEBUG
            std::cout << entry.path().stem() << std::endl;
#endif
            if (entry.path().extension() == ".save.dgr" || entry.path().extension() == ".dgr") {

                if (fileName == entry.path().stem()) {
                    gameStatePtr->getScene("AreYouSureScene")->message(
                          "LoadGameScene: delete, " + std::string(entry.path()));

                    gameStatePtr->pushSceneToStack("AreYouSureScene");
                }
            }
        }

    } else if (buttonName == "SelectSave0") {
        selectedGame = currentLeftSaveIndex;

    } else if (buttonName == "SelectSave1") {
        selectedGame = currentLeftSaveIndex + 1;

    } else if (buttonName == "SelectSave2") {
        selectedGame = currentLeftSaveIndex + 2;

    } else if (buttonName == "SelectSave3") {
        selectedGame = currentLeftSaveIndex + 3;

    } else if (buttonName == "Ready") {
        if (selectedGame >= (int) loadedGames.size()) {
            gameStatePtr->addOnScreenMessage("No data found, please select another save.");
            return;
        }
        auto &battle = loadedGames[loadedGameNames[selectedGame]];
        std::shared_ptr<BattleScene> battleScene = std::dynamic_pointer_cast<BattleScene>(
              gameStatePtr->getScene("BattleScene"));
        if (!battleScene) {
            std::cerr << "CharacterSelectScene::pressButton: error: \"BattleScene\" not found" << std::endl;
            exit(404);
        }
        battleScene->reset();
        battleScene->setBattleLog(battle);
        battleScene->rerunBattleFromStart();

        gameStatePtr->popSceneFromStack();
        gameStatePtr->pushSceneToStack("BattleScene");
    }
}

void LoadGameScene::onPushToStack() {
    loadedGames = {};
    loadedGameNames = {};

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    std::string dir = "../src/io/saves/";
    auto dirIt = std::filesystem::directory_iterator(dir);
    auto allSaves = Utilities::getAllFileNamesInDir(dir, ".dgr");
    for (const auto &save : allSaves) {
#if DGR_DEBUG
        std::cout << entry.path() << std::endl;
#endif
        std::shared_ptr<BattleLog> battle =
              BattleLog::loadBattle(gameStatePtr, save);

        if (battle) {
            auto name = std::string(save.stem());
            loadedGames[name] = battle;
            loadedGameNames.push_back(name);
        }
    }

    if (loadedGameNames.empty()) {
        gameStatePtr->popSceneFromStack();
        gameStatePtr->pushSceneToStack("CharacterSelectScene");
    }
}

std::string LoadGameScene::message(const std::string &data) {

    if (data.substr(0, 5) != "yes: ") {
        return data;
    }

    if (data.substr(5, 8) == "delete, ") {
        /// delete save
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        std::string filePath = data.substr(13, data.size() - 13);
        std::string dir = "../src/io/saves/";
        auto dirIt = std::filesystem::directory_iterator(dir);
        auto allSaveNames = Utilities::getAllFileNamesInDir(dir, ".save.dgr");
        for (const auto &saveName : allSaveNames) {

            if (filePath == saveName) {
                std::cout << saveName.stem() << std::endl;

                std::cout << "deleting.." << std::endl;

                std::filesystem::remove(saveName);
                return data;
            }
        }

        /// reload
        gameStatePtr->popSceneFromStack();
        gameStatePtr->pushSceneToStack("LoadGameScene");

    }
    return data;
}

void LoadGameScene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer) const {
    spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f, glm::vec2(0), size,
                               glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);

        for (int i = 0; i < 4; i++) {
            if (button->getName() == "SelectSave" + std::to_string(i)) {
                if (i == selectedGame - currentLeftSaveIndex) {
                    button->drawColor(spriteRenderer, textRenderer, glm::vec3(1, 0, 0), 0.9f);
                }
                auto position = button->getPosition();
                auto size = button->getSize();
                drawLoadedGame(spriteRenderer, textRenderer, i, position, size);
            }
        }
    }
}

void LoadGameScene::drawLoadedGame(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                                   const std::unique_ptr<TextRenderer> &textRenderer, int index,
                                   glm::vec2 position, glm::vec2 size) const {

    if (index + currentLeftSaveIndex >= (int) loadedGames.size()) {
        return;
    }

    auto gameName = loadedGameNames[index + currentLeftSaveIndex];
    auto game = loadedGames.at(gameName);
    auto state = game->getState();
    auto heroes = std::get<0>(state);

    textRenderer->drawText(gameName, 0.0f, position, size);
    for (int i = 0; i < (int) heroes->size(); i++) {
        auto hero = heroes->at(i);

        int nX = (int) size.x / 32;
        int xI = i % nX;
        int yI = i / nX;
        double dPos = size.x / (double) nX;
        double xPos = position.x + xI * dPos + (dPos - 32.0) / 2.0;
        double yPos = position.y + yI * dPos + (dPos - 32.0) / 2.0 + 16;

        hero->setPosition(glm::vec2(xPos, yPos));
        hero->drawHeroOnly(spriteRenderer);

    }
}


}
