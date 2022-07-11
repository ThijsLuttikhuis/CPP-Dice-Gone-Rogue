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
#include "utilities/BattleLog.h"

namespace DGR {

LoadGameScene::LoadGameScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("LoadGameScene", std::move(gameState)) {

    double width = DGR_WIDTH;
    double height = DGR_HEIGHT;

    double buttonWidth = width * 0.2;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    float i = 8;

    auto button1 = std::make_shared<Button>("Text", glm::vec2(width / 2 - buttonWidth, buttonDistance / 2),
                                            glm::vec2(buttonWidth * 2, buttonHeight / 2));
    button1->setText("Select three characters");
    buttons.push_back(button1);

    auto button2 = std::make_shared<Button>("Ready",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button2->setText("Load save file");
    buttons.push_back(button2);

    auto button9 = std::make_shared<Button>("DeleteSave",
                                            glm::vec2(width / 2 - buttonWidth / 2 + buttonWidth * 1.1,
                                                      i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button9->setText("Delete save file");
    buttons.push_back(button9);

    auto button10 = std::make_shared<Button>("Return",
                                             glm::vec2(width / 2 - buttonWidth / 2 - buttonWidth * 1.1,
                                                       i * buttonDistance),
                                             glm::vec2(buttonWidth, buttonHeight));
    button10->setText("Return to main menu");
    buttons.push_back(button10);

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

    auto button3 = std::make_shared<Button>("ScrollLeft",
                                            glm::vec2(leftButtonX, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight));
    button3->setText(" <<");
    buttons.push_back(button3);

    auto button4 = std::make_shared<Button>("ScrollRight",
                                            glm::vec2(rightButtonX, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight));
    button4->setText(" >>");
    buttons.push_back(button4);

    auto button5 = std::make_shared<Button>("SelectSave0",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button5->setText("");
    buttons.push_back(button5);

    auto button6 = std::make_shared<Button>("SelectSave0",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button6->setText("");
    buttons.push_back(button6);

    auto button7 = std::make_shared<Button>("SelectSave0",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button7->setText("");
    buttons.push_back(button7);

    auto button8 = std::make_shared<Button>("SelectSave0",
                                            glm::vec2(midButtonStart + i * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button8->setText("");
    buttons.push_back(button8);

}

void LoadGameScene::handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos) {
    auto dice = character->getDice();
    dice->setCurrentFaceHover(dice->isMouseHovering(xPos, yPos, Dice::currentFacePos));

    if (character->isMouseHovering(xPos, yPos)) {
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
    //TODO: show preview of save game
//    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
//    for (auto &hero : gameStatePtr->getAllHeroes()) {
//        handleMousePosition(hero, xPos, yPos);
//
//        auto spell = hero->getSpell();
//        if (spell) {
//            spell->setHover(spell->isMouseHovering(xPos, yPos));
//        }
//    }
}

void LoadGameScene::alignSavesPositions() {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    const int width = gameStatePtr->getWindow()->getWidth();
    const int center = width / 2;
    const int dWidth = 48;
    const int maxTotalWidth = maxSavesOnRow * (dWidth + 32);

    int startLeft;
    int totalWidth;
    int left, up;
    totalWidth = 0;

    auto allHeroes = gameStatePtr->getAllHeroes();
    int allHeroesSize = allHeroes.size();
    for (int i = currentLeftSaveIndex; i < allHeroesSize; i++) {
        totalWidth += dWidth + (int) allHeroes[i]->getSize().x;
    }

    totalWidth = std::min(totalWidth, maxTotalWidth);

    startLeft = (int) center - totalWidth / 2;
    left = startLeft;
    up = 9 * 16;

    for (int i = 0; i < allHeroesSize; i++) {
        auto hero = allHeroes[i];
        glm::vec2 size = hero->getSize();

        if (i < currentLeftSaveIndex) {
            hero->setPosition(-left, -up);
            continue;
        }
        if ((int) size.x + left - startLeft > maxTotalWidth) {
            hero->setPosition(-left, -up);
            continue;
        }
        glm::vec2 targetPos(left, (float) up - size.y);
        hero->setPosition(targetPos);

        left += dWidth + (int) hero->getSize().x;
    }
}

void LoadGameScene::reset() {
    loadedGames = {};
    selectedGame = 0;

    maxSavesOnRow = 4;
    currentLeftSaveIndex = 0;
}

void LoadGameScene::update(double dt) {
    (void) dt;


//    auto allHeroes = gameState->getAllHeroes();
//    if (allHeroes.empty()) {
//        auto allHeroes_ = gameState->getAllHeroes();
//        for (auto &hero : allHeroes_) {
//            auto copy = hero->makeCopy(true);
//            allHeroes.push_back(copy);
//        }
//    }

    alignSavesPositions();

}

void LoadGameScene::pressButton(const std::shared_ptr<Button>& button) {
    std::cout << "pressed a button!" << std::endl;

    auto &buttonName = button->getName();

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    if (buttonName == "ScrollRight") {
        currentLeftSaveIndex++;
        currentLeftSaveIndex = std::min(currentLeftSaveIndex,
                                        (int) gameStatePtr->getAllHeroes().size() - maxSavesOnRow);
    } else if (buttonName == "ScrollLeft") {
        currentLeftSaveIndex--;
        currentLeftSaveIndex = std::max(currentLeftSaveIndex, 0);
    } else if (buttonName == "Randomize") {

    } else if (buttonName == "Return") {
        gameStatePtr->popSceneFromStack();

    } else if (buttonName == "SelectSave0") {

    } else if (buttonName == "SelectSave1") {

    } else if (buttonName == "SelectSave2") {

    } else if (buttonName == "SelectSave3") {

    } else if (buttonName == "Ready") {
        auto &battle = loadedGames[selectedGame];
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

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    std::string dir = "../src/io/saves/";
    auto dirIt = std::filesystem::directory_iterator(dir);
    for (const auto &entry : dirIt) {
#if DGR_DEBUG
        std::cout << entry.path() << std::endl;
#endif
        if (entry.path().extension() == ".save.dgr" || entry.path().extension() == ".dgr") {
            std::shared_ptr<BattleLog> battle =
                  BattleLog::loadBattle(gameStatePtr, entry.path());

            if (battle) {
                loadedGames.push_back(battle);
            }
        }
    }

    if (loadedGames.empty()) {
        gameStatePtr->popSceneFromStack();
        gameStatePtr->pushSceneToStack("CharacterSelectScene");
    }
}

void LoadGameScene::initialize() {
}

void LoadGameScene::render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                           const std::shared_ptr<TextRenderer> &textRenderer) {
    spriteRenderer->drawSprite("box", 1.0f, glm::vec2(0), size,
                               0.0f, glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

//    for (auto &hero : gameStatePtr->getAllHeroes()) {
//        hero->draw(spriteRenderer, textRenderer);
//    }

//    for (auto &hero : gameStatePtr->getAllHeroes()) {
//        hero->setHoverMouse(true);
//        hero->drawHover(spriteRenderer, textRenderer);
//    }

}

}
