//
// Created by thijs on 21-06-22.
//

#include <utilities/Random.h>
#include "CharacterSelectScene.h"
#include "ui/Button.h"
#include "GameStateManager.h"
#include "BattleScene.h"

namespace DGR {

CharacterSelectScene::CharacterSelectScene(GameStateManager* gameState)
      : Scene("CharacterSelectScene", gameState) {

    double width = DGR_WIDTH;
    double height = DGR_HEIGHT;

    double buttonWidth = width * 0.2;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    float i = 8;

    auto* button1 = new Button("Text", {width / 2 - buttonWidth, buttonDistance / 2},
                               {buttonWidth * 2, buttonHeight / 2});
    button1->setText("Select three characters");
    buttons.push_back(button1);

    auto* button2 = new Button("Ready", {width / 2 - buttonWidth / 2, i * buttonDistance},
                               {buttonWidth, buttonHeight});
    button2->setText("Ready");
    buttons.push_back(button2);

    float grayValue = 0.4f;

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

    auto* button3 = new Button("ScrollLeft", {leftButtonX, leftRightButtonY},
                               {leftRightButtonWidth, leftRightButtonHeight});
    button3->setText(" <<");
    buttons.push_back(button3);

    auto* button4 = new Button("ScrollRight", {rightButtonX, leftRightButtonY},
                               {leftRightButtonWidth, leftRightButtonHeight});
    button4->setText(" >>");
    buttons.push_back(button4);

    auto* button5 = new Button("SelectHero0", {midButtonStart + i++ * midButtonWidth, leftRightButtonY},
                               {midButtonWidth, leftRightButtonHeight}, false);
    button5->setText("");
    buttons.push_back(button5);

    auto* button6 = new Button("SelectHero1", {midButtonStart + i++ * midButtonWidth, leftRightButtonY},
                               {midButtonWidth, leftRightButtonHeight}, false);
    button6->setText("");
    buttons.push_back(button6);

    auto* button7 = new Button("SelectHero2", {midButtonStart + i++ * midButtonWidth, leftRightButtonY},
                               {midButtonWidth, leftRightButtonHeight}, false);
    button7->setText("");
    buttons.push_back(button7);

    auto* button8 = new Button("SelectHero3", {midButtonStart + i * midButtonWidth, leftRightButtonY},
                               {midButtonWidth, leftRightButtonHeight}, false);
    button8->setText("");
    buttons.push_back(button8);
}

void CharacterSelectScene::handleMousePosition(Character* character, double xPos, double yPos) {
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

void CharacterSelectScene::alignCharacterPositions(double dt) {
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

    alignCharacterPositions(dt);

}

void CharacterSelectScene::render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
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

void CharacterSelectScene::pressButton(Button* button) {
    std::cout << "pressed a button!" << std::endl;

    float heroYPos = 128;
    float heroXPos = 112;
    float heroDXPos = 80;

    if (button->getName() == "ScrollRight") {
        currentLeftCharacterIndex++;
        currentLeftCharacterIndex = std::min(currentLeftCharacterIndex,
                                             (int) gameState->getAllHeroes().size() - maxCharactersOnRow);
    } else if (button->getName() == "ScrollLeft") {
        currentLeftCharacterIndex--;
        currentLeftCharacterIndex = std::max(currentLeftCharacterIndex, 0);
    } else if (button->getName() == "SelectHero0") {
        handleHeroesMouseButton(heroXPos, heroYPos);

    } else if (button->getName() == "SelectHero1") {
        handleHeroesMouseButton(heroXPos + heroDXPos, heroYPos);

    } else if (button->getName() == "SelectHero2") {
        handleHeroesMouseButton(heroXPos + heroDXPos * 2, heroYPos);

    } else if (button->getName() == "SelectHero3") {
        handleHeroesMouseButton(heroXPos + heroDXPos * 3, heroYPos);

    } else if (button->getName() == "Ready") {
        if ((int) selectedHeroes.size() == maxSelect) {
            BattleScene* battleScene = dynamic_cast<BattleScene*>(gameState->getScene("BattleScene"));
            if (!battleScene) {
                std::cerr << "CharacterSelectScene::pressButton: error: \"BattleScene\" not found" << std::endl;
                exit(404);
            }

            std::vector<Character*> battleHeroes;
            std::vector<Character*> battleEnemies;

            for (auto &hero : selectedHeroes) {
                auto copy = hero->makeCopy(true);
                copy->setPosition(0, 0);
                battleHeroes.push_back(copy);
            }

            auto allEnemies = gameState->getAllEnemies();
            for (int i = 0; i < 3; i++) {
                int n = allEnemies.size();
                int rng = Random::randInt(0, n - 1);
                auto enemy = allEnemies[rng]->makeCopy(true);
                battleEnemies.push_back(enemy);
            }

            battleScene->reset();
            battleScene->setHeroes(battleHeroes);
            battleScene->setEnemies(battleEnemies);

            gameState->popSceneFromStack();
            gameState->addSceneToStack("BattleScene");
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
