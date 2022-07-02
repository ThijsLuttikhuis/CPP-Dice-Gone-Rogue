//
// Created by thijs on 21-06-22.
//

#include <utilities/Random.h>
#include "CharacterSelectScene.h"
#include "ui/Button.h"
#include "GameStateManager.h"
#include "BattleScene.h"
#include <set>

namespace DGR {

CharacterSelectScene::CharacterSelectScene(std::shared_ptr<GameStateManager> gameState)
      : Scene("CharacterSelectScene", gameState) {

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
    button2->setText("Ready");
    buttons.push_back(button2);

    auto button9 = std::make_shared<Button>("Randomize",
                                            glm::vec2(width / 2 - buttonWidth / 2 + buttonWidth * 1.1,
                                                      i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button9->setText("Randomize");
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

    auto button5 = std::make_shared<Button>("SelectHero0",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button5->setText("");
    buttons.push_back(button5);

    auto button6 = std::make_shared<Button>("SelectHero1",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button6->setText("");
    buttons.push_back(button6);

    auto button7 = std::make_shared<Button>("SelectHero2",
                                            glm::vec2(midButtonStart + i++ * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button7->setText("");
    buttons.push_back(button7);

    auto button8 = std::make_shared<Button>("SelectHero3",
                                            glm::vec2(midButtonStart + i * midButtonWidth, leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button8->setText("");
    buttons.push_back(button8);


}

void CharacterSelectScene::handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos) {
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

void CharacterSelectScene::handleMouseButton(double xPos, double yPos) {

    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
            return;
        }
    }

    handleHeroesMouseButton(xPos, yPos);
}

void CharacterSelectScene::handleMousePosition(double xPos, double yPos) {
    for (auto &hero : gameState->getAllHeroes()) {
        handleMousePosition(hero, xPos, yPos);

        auto spell = hero->getSpell();
        if (spell) {
            spell->setHover(spell->isMouseHovering(xPos, yPos));
        }
    }
}

void CharacterSelectScene::alignCharacterPositions() {
    const int width = gameState->getWindow()->getWidth();
    const int center = width / 2;
    const int dWidth = 48;
    const int maxTotalWidth = maxCharactersOnRow * (dWidth + 32);

    int startLeft;
    int totalWidth;
    int left, up;
    totalWidth = 0;

    auto allHeroes = gameState->getAllHeroes();
    int allHeroesSize = allHeroes.size();
    for (int i = currentLeftCharacterIndex; i < allHeroesSize; i++) {
        totalWidth += dWidth + (int) allHeroes[i]->getSize().x;
    }

    totalWidth = std::min(totalWidth, maxTotalWidth);

    startLeft = (int) center - totalWidth / 2;
    left = startLeft;
    up = 9 * 16;

    for (int i = 0; i < allHeroesSize; i++) {
        auto hero = allHeroes[i];
        glm::vec2 size = hero->getSize();

        if (i < currentLeftCharacterIndex) {
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

void CharacterSelectScene::reset() {
    Scene::reset();
}

void CharacterSelectScene::update(double dt) {
    auto allHeroes = gameState->getAllHeroes();
    if (allHeroes.empty()) {
        auto allHeroes_ = gameState->getAllHeroes();
        for (auto &hero : allHeroes_) {
            auto copy = hero->makeCopy(true);
            allHeroes.push_back(copy);
        }
    }

    alignCharacterPositions();

}

void CharacterSelectScene::render(std::shared_ptr<SpriteRenderer> spriteRenderer,
                                  std::shared_ptr<TextRenderer> textRenderer) {
    spriteRenderer->drawSprite("box", 1.0f, glm::vec2(0), size,
                               0.0f, glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }

    for (auto &hero : gameState->getAllHeroes()) {
        hero->draw(spriteRenderer, textRenderer);
    }

    for (auto &hero : gameState->getAllHeroes()) {
        hero->setHoverMouse(true);
        hero->drawHover(spriteRenderer, textRenderer);
    }

    for (auto &hero : selectedHeroes) {
        hero->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }


}

void CharacterSelectScene::pressButton(std::shared_ptr<Button> button) {
    std::cout << "pressed a button!" << std::endl;

    float heroYPos = 128;
    float heroXPos = 112;
    float heroDXPos = 80;

    auto &buttonName = button->getName();

    if (buttonName == "ScrollRight") {
        currentLeftCharacterIndex++;
        currentLeftCharacterIndex = std::min(currentLeftCharacterIndex,
                                             (int) gameState->getAllHeroes().size() - maxCharactersOnRow);
    } else if (buttonName == "ScrollLeft") {
        currentLeftCharacterIndex--;
        currentLeftCharacterIndex = std::max(currentLeftCharacterIndex, 0);
    } else if (buttonName == "Randomize") {
        auto allHeroes = gameState->getAllHeroes();
        std::set<int> rngs;
        while ((int) rngs.size() < maxSelect) {
            int rng = Random::randInt(0, (int) allHeroes.size() - 1);
            rngs.insert(rng);
        }
        selectedHeroes = {};
        std::string message = "selected ";

        for (auto rng : rngs) {
            auto hero = allHeroes[rng];
            message += hero->getName() + ", ";
            selectedHeroes.push_back(hero);
        }

        gameState->addOnScreenMessage(message);

    } else if (buttonName == "Return") {
        gameState->popSceneFromStack();

    } else if (buttonName == "SelectHero0") {
        handleHeroesMouseButton(heroXPos, heroYPos);

    } else if (buttonName == "SelectHero1") {
        handleHeroesMouseButton(heroXPos + heroDXPos, heroYPos);

    } else if (buttonName == "SelectHero2") {
        handleHeroesMouseButton(heroXPos + heroDXPos * 2, heroYPos);

    } else if (buttonName == "SelectHero3") {
        handleHeroesMouseButton(heroXPos + heroDXPos * 3, heroYPos);

    } else if (buttonName == "Ready") {
        if ((int) selectedHeroes.size() == maxSelect) {
            std::shared_ptr<BattleScene> battleScene = std::dynamic_pointer_cast<BattleScene>(gameState->getScene("BattleScene"));
            if (!battleScene) {
                std::cerr << "CharacterSelectScene::pressButton: error: \"BattleScene\" not found" << std::endl;
                exit(404);
            }

            std::vector<std::shared_ptr<Character>> battleHeroes;
            std::vector<std::shared_ptr<Character>> battleEnemies;

            for (auto &hero : selectedHeroes) {
                auto copy = hero->makeCopy(true);
                copy->setPosition(0, 0);
                battleHeroes.push_back(copy);
            }

            auto allEnemies = gameState->getAllEnemies();
            for (auto &enemy : allEnemies) {
                auto copy = enemy->makeCopy(true);
                battleEnemies.push_back(copy);
            }

            battleScene->reset();
            battleScene->setHeroes(battleHeroes);
            battleScene->setEnemies(battleEnemies);

            gameState->popSceneFromStack();
            gameState->addSceneToStack("BattleScene");
        } else {
            gameState->addOnScreenMessage("Please select three characters!");
        }
    }
}

bool CharacterSelectScene::handleHeroesMouseButton(double xPos, double yPos) {
    for (auto &hero : gameState->getAllHeroes()) {
        if (hero->isMouseHovering(xPos, yPos, true)) {

            bool removedCharacter = false;
            for (auto it = selectedHeroes.begin(); it != selectedHeroes.end(); it++) {
                if (hero == *it) {
                    selectedHeroes.erase(it);
                    removedCharacter = true;
                    break;
                }
            }

            if (!removedCharacter) {
                if ((int) selectedHeroes.size() >= maxSelect) {
                    selectedHeroes.erase(selectedHeroes.begin());
                }

                selectedHeroes.push_back(hero);
            }

            return true;
        }
    }

    return false;
}

}
