//
// Created by thijs on 21-06-22.
//

#include <utilities/Random.h>
#include "CharacterSelectScene.h"
#include "ui/Button.h"
#include "GameStateManager.h"
#include "BattleScene.h"
#include <set>
#include <utility>

namespace DGR {

CharacterSelectScene::CharacterSelectScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("CharacterSelectScene", std::move(gameState)) {

    float leftRightButtonWidth = 24;
    float leftRightButtonHeight = 182;
    float leftRightButtonY = 48;

    float leftButtonX = 63;
    float rightButtonX = 409;

    float nMidButton = 4;
    float midButtonStart = leftButtonX + leftRightButtonWidth;
    float midButtonEnd = rightButtonX;
    float midButtonWidth = (midButtonEnd - midButtonStart) / nMidButton;
    float midButtonWidth_3 = (midButtonEnd - midButtonStart) / 3;
    float topBotButtonHeight = 20;
    int i = 0;

    /// -------------

    auto button3 = std::make_unique<Button>("ScrollLeft",
                                            glm::vec2(leftButtonX - 4, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight),
                                            glm::vec3(0.4f),
                                            0.0f, true, true);
    button3->setText(" <<");
    button3->setKeyboardKey(GLFW_KEY_COMMA);
    buttons.push_back(std::move(button3));

    auto button4 = std::make_unique<Button>("ScrollRight",
                                            glm::vec2(rightButtonX + 4, leftRightButtonY),
                                            glm::vec2(leftRightButtonWidth, leftRightButtonHeight),
                                            glm::vec3(0.4f),
                                            0.0f, true, true);
    button4->setText(" >>");
    button4->setKeyboardKey(GLFW_KEY_PERIOD);
    buttons.push_back(std::move(button4));

    auto button5 = std::make_unique<Button>("SelectHero0",
                                            glm::vec2(midButtonStart + (float) i++ * midButtonWidth,
                                                      leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button5->setText("");
    button5->setKeyboardKey(GLFW_KEY_1);
    buttons.push_back(std::move(button5));

    auto button6 = std::make_unique<Button>("SelectHero1",
                                            glm::vec2(midButtonStart + (float) i++ * midButtonWidth,
                                                      leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button6->setText("");
    button6->setKeyboardKey(GLFW_KEY_2);
    buttons.push_back(std::move(button6));

    auto button7 = std::make_unique<Button>("SelectHero2",
                                            glm::vec2(midButtonStart + (float) i++ * midButtonWidth,
                                                      leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button7->setText("");
    button7->setKeyboardKey(GLFW_KEY_3);
    buttons.push_back(std::move(button7));

    auto button8 = std::make_unique<Button>("SelectHero3",
                                            glm::vec2(midButtonStart + (float) i * midButtonWidth,
                                                      leftRightButtonY),
                                            glm::vec2(midButtonWidth, leftRightButtonHeight), false);
    button8->setText("");
    button8->setKeyboardKey(GLFW_KEY_4);
    buttons.push_back(std::move(button8));

    /// -------------

    auto button1 = std::make_unique<Button>("Text",
                                            glm::vec2(leftButtonX + leftRightButtonWidth + 1,
                                                      leftRightButtonY - topBotButtonHeight - 4),
                                            glm::vec2(rightButtonX - leftButtonX - leftRightButtonWidth,
                                                      topBotButtonHeight));
    button1->setText("Select " + Utilities::num2OneTwoThreeString(maxSelect) + " characters");
    buttons.push_back(std::move(button1));

    auto button2 = std::make_unique<Button>("Ready",
                                            glm::vec2(leftButtonX + leftRightButtonWidth + midButtonWidth_3,
                                                      leftRightButtonHeight + leftRightButtonY + 4),
                                            glm::vec2(midButtonWidth_3, topBotButtonHeight));
    button2->setText("Ready");
    button2->setKeyboardKey(GLFW_KEY_ENTER);
    buttons.push_back(std::move(button2));

    auto button9 = std::make_unique<Button>("Randomize",
                                            glm::vec2(leftButtonX + leftRightButtonWidth + midButtonWidth_3 * 2,
                                                      leftRightButtonHeight + leftRightButtonY + 4),
                                            glm::vec2(midButtonWidth_3, topBotButtonHeight));
    button9->setText("Randomize");
    button9->setKeyboardKey(GLFW_KEY_R);
    buttons.push_back(std::move(button9));

    auto button10 = std::make_unique<Button>("Return",
                                             glm::vec2(leftButtonX + leftRightButtonWidth,
                                                       leftRightButtonHeight + leftRightButtonY + 4),
                                             glm::vec2(midButtonWidth_3, topBotButtonHeight));
    button10->setText("Return to main menu");
    button10->setKeyboardKey(GLFW_KEY_ESCAPE);
    buttons.push_back(std::move(button10));
}

void CharacterSelectScene::handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos) {
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
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
    for (auto &hero : gameStatePtr->getAllHeroes()) {
        handleMousePosition(hero, xPos, yPos);

        auto spell = hero->getSpell();
        if (spell) {
            spell->setHover(spell->isMouseHovering(xPos, yPos));
        }
    }
}

void CharacterSelectScene::alignCharacterPositions() {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    const int width = gameStatePtr->getWindow()->getWidth();
    const int center = width / 2;
    const int dWidth = 48;
    const int maxTotalWidth = maxCharactersOnRow * (dWidth + 32);

    int startLeft;
    int totalWidth;
    int left, up;
    totalWidth = 0;

    auto allHeroes = gameStatePtr->getAllHeroes();
    int allHeroesSize = allHeroes.size();
    for (int i = currentLeftCharacterIndex; i < allHeroesSize; i++) {
        totalWidth += dWidth + (int) allHeroes[i]->getSize().x;
    }

    totalWidth = std::min(totalWidth, maxTotalWidth);

    startLeft = (int) center - totalWidth / 2;
    left = startLeft;
    up = 10 * 16;

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

void CharacterSelectScene::update(double dt) {
    t += dt;

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    for (auto &button : buttons) {
        if (button->getName() == "ScrollRight") {
            button->setDoBlink((int) gameStatePtr->getAllHeroes().size() >
                               currentLeftCharacterIndex + maxCharactersOnRow);
        } else if (button->getName() == "ScrollLeft") {
            button->setDoBlink(currentLeftCharacterIndex > 0);
        }

        button->update(t);
    }

    alignCharacterPositions();
}

void CharacterSelectScene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                                  const std::unique_ptr<TextRenderer> &textRenderer) const {

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f, glm::vec2(0), size,
                                glm::vec3(0.2f), 0.9f);

    renderDefaults(spriteRenderer, textRenderer);

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    for (auto &hero : gameStatePtr->getAllHeroes()) {
        if (hero->getPosition().x < 0) continue;

        hero->drawShadow(spriteRenderer, textRenderer);
        hero->draw(spriteRenderer, textRenderer);

    }

    for (auto &hero : gameStatePtr->getAllHeroes()) {
        if (hero->getPosition().x < 0) continue;

        hero->setHoverMouse(true);
        hero->drawHover(spriteRenderer, textRenderer);
    }

    for (auto &hero : selectedHeroes) {
        if (hero->getPosition().x < 0) continue;

        hero->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

}

void CharacterSelectScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;

    float heroYPos = 128;
    float heroXPos = 112;
    float heroDXPos = 80;

    auto &buttonName = button->getName();

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    if (buttonName == "ScrollRight") {
        currentLeftCharacterIndex++;
        currentLeftCharacterIndex = std::min(currentLeftCharacterIndex,
                                             (int) gameStatePtr->getAllHeroes().size() - maxCharactersOnRow);
    } else if (buttonName == "ScrollLeft") {
        currentLeftCharacterIndex--;
        currentLeftCharacterIndex = std::max(currentLeftCharacterIndex, 0);
    } else if (buttonName == "Randomize") {
        auto allHeroes = gameStatePtr->getAllHeroes();
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

        gameStatePtr->addOnScreenMessage(message);

    } else if (buttonName == "Return") {
        gameStatePtr->popSceneFromStack();

    } else if (buttonName == "SelectHero0") {
        handleHeroesMouseButton(heroXPos, heroYPos);

    } else if (buttonName == "SelectHero1") {
        handleHeroesMouseButton(heroXPos + heroDXPos, heroYPos);

    } else if (buttonName == "SelectHero2") {
        handleHeroesMouseButton(heroXPos + heroDXPos * 2, heroYPos);

    } else if (buttonName == "SelectHero3") {
        handleHeroesMouseButton(heroXPos + heroDXPos * 3, heroYPos);

    } else if (buttonName == "Ready") {
        if ((int) selectedHeroes.size() != maxSelect) {
            gameStatePtr->addOnScreenMessage("Please select " +
                                             Utilities::num2OneTwoThreeString(maxSelect) +
                                             " characters!");
            return;
        }

        auto &inventory = gameStatePtr->getInventory();
        inventory->setHeroes(selectedHeroes);

        gameStatePtr->popSceneFromStack();
        gameStatePtr->pushSceneToStack("LevelSelectScene");
    }
}

bool CharacterSelectScene::handleHeroesMouseButton(double xPos, double yPos) {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    for (auto &hero : gameStatePtr->getAllHeroes()) {
        if (hero->isMouseHovering_(xPos, yPos, Character::hoverType::extendedBox)) {

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
