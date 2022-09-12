//
// Created by thijs on 17-06-22.
//

#include <utilities/Random.h>

#include <utility>
#include "BattleScene.h"
#include "ui/Button.h"
#include "GameStateManager.h"
#include "LevelSelectScene.h"
#include "gamelogic/BattleController.h"

namespace DGR {

BattleScene::BattleScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("BattleScene", std::move(gameState)) {

    auto button1 = std::make_unique<Button>("leftMainButton", glm::vec2(304, 232),
                                            glm::vec2(80, 15), glm::vec3(0.9));
    button1->setText("2 rerolls left");
    button1->setKeyboardKey(GLFW_KEY_COMMA);
    buttons.push_back(std::move(button1));

    auto button2 = std::make_unique<Button>("rightMainButton", glm::vec2(400, 232),
                                            glm::vec2(80, 15), glm::vec3(0.9));
    button2->setText("done rolling");
    button2->setKeyboardKey(GLFW_KEY_PERIOD);
    buttons.push_back(std::move(button2));

    auto button3 = Scene::makeDefaultButton("Settings", glm::vec2(112, 8), glm::vec2(12, 12));

    button3->setText("S");
    buttons.push_back(std::move(button3));

    auto button4 = std::make_unique<Button>("help", glm::vec2(88, 8),
                                            glm::vec2(12, 12));
    button4->setText("?");
    buttons.push_back(std::move(button4));
}

std::shared_ptr<BattleScene> BattleScene::getSharedFromThis() {
    return shared_from_this();
}

void BattleScene::handleCharacterMousePosition(std::shared_ptr<Character> character, double xPos, double yPos) {
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

void BattleScene::handleMousePosition(double xPos, double yPos) {
    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();

    for (auto &hero : battleController->getHeroes()) {
        handleCharacterMousePosition(hero, xPos, yPos);

        auto spell = hero->getSpell();
        if (spell) {
            spell->setHover(spell->isMouseHovering(xPos, yPos));
        }
    }

    for (auto &enemy : battleController->getEnemies()) {
        handleCharacterMousePosition(enemy, xPos, yPos);
    }
}

void BattleScene::alignCharacterPositions(double dt) {
    updateCharacterKeyPresses();

    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
    auto &battleController = gameStatePtr->getBattleController();

    const int width = gameStatePtr->getWindow()->getWidth();
    const int center = width / 2;
    const float moveSpeed = 100.0f;
    const float maxDPos = moveSpeed * (float) dt;
    const int dWidth = 16;
    const int maxTotalWidth = 6 * (dWidth + 32);

    int startLeft;
    int totalWidth;
    int left, up;
    bool isHeroes = false;
    for (auto &characters : {battleController->getHeroes(), battleController->getEnemies()}) {
        isHeroes = !isHeroes;
        totalWidth = 0;
        for (auto &character : characters) {
            if (!character->isDead()) {
                totalWidth += dWidth + (int) character->getSize().x;
            }
        }

        totalWidth = std::min(totalWidth, maxTotalWidth);

        startLeft = isHeroes ? (int) (center * 0.6) - totalWidth / 2 : (int) (center * 1.4) - totalWidth / 2;
        left = startLeft;
        up = isHeroes ? 13 * 16 : 8.5 * 16;

        for (auto &character : characters) {
            if (!character->isDead()) {
                glm::vec2 size = character->getSize();
                if ((int) size.x + left - startLeft > maxTotalWidth) {
                    std::cout << left << startLeft << maxTotalWidth << std::endl;
                    std::cout << "too many characters" << std::endl;
                    character->setPosition(-left, -up);
                    continue;
                }
                glm::vec2 targetPos(left, (float) up - size.y);
                glm::vec2 pos = character->getPosition();
                glm::vec2 dPos(targetPos - pos);

                if (glm::length(dPos) > maxDPos && glm::length(dPos) < moveSpeed) {
                    dPos = glm::normalize(dPos) * maxDPos;
                    character->setPosition(pos + dPos);
                } else {
                    character->setPosition(targetPos);
                }
                left += dWidth + (int) character->getSize().x;
            } else {
                character->setPosition(-left, -up);
            }
        }
    }
}

void BattleScene::update(double dt) {
    updateDefaults(dt);
    alignCharacterPositions(dt);

    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();
    battleController->update(dt);

    updateButtons();

}

void BattleScene::handleMouseButton(double xPos, double yPos) {
    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();

    handleMouseButtonDefault(xPos, yPos);

    for (auto &hero : battleController->getHeroes()) {
        if (hero->isMouseHovering_(xPos, yPos, Character::hoverType::extendedBox)) {
            battleController->clickCharacter(hero);
            return;
        }

        auto spell = hero->getSpell();
        if (spell) {
            if (spell->isMouseHovering(xPos, yPos)) {
                battleController->clickSpell(spell);
                return;
            }
        }
    }

    for (auto &enemy : battleController->getEnemies()) {
        if (enemy->isMouseHovering_(xPos, yPos, Character::hoverType::extendedBox)) {
            battleController->clickCharacter(enemy);
            return;
        }
    }

}

void BattleScene::updateCharacterKeyPresses() {
    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();

    int count = 0;
    for (auto &hero : battleController->getHeroes()) {
        if (!hero->isDead()) {
            hero->setKeyboardKey(GLFW_KEY_1 + count++);
        } else {
            hero->setKeyboardKey();
        }
    }
    count = 0;
    for (auto &enemy : battleController->getEnemies()) {
        if (!enemy->isDead()) {
            enemy->setKeyboardKey(GLFW_KEY_1 + count++);
        } else {
            enemy->setKeyboardKey();
        }
    }
}

void BattleScene::handleKeyboard(int key, int action, const std::unique_ptr<std::vector<bool>> &keysPressed) {
    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();

    handleKeyboardDefault(key, action, keysPressed);

    if (action == GLFW_RELEASE) {
        if (keysPressed->at(GLFW_KEY_LEFT_SHIFT)) {
            for (auto &enemy : battleController->getEnemies()) {
                if (enemy->isKeyPressed(key)) {
                    battleController->clickCharacter(enemy);
                    break;
                }
            }
        } else {
            for (auto &hero : battleController->getHeroes()) {
                if (hero->isKeyPressed(key)) {
                    battleController->clickCharacter(hero);
                    break;
                }
            }
        }
    }
}

std::string BattleScene::message(const std::string &data) {
    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();

    std::cout << data << std::endl;

    if (data.substr(0, 5) != "yes: ") {
        return data;
    }
    if (data == "yes: nextGameState") {
        battleController->setNextGameState();
    }
    return data;
}


void BattleScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;

    if (pressDefaultButton(button)) {
        return;
    }

    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();

    if (button->getName() == "leftMainButton") {
        battleController->pressLeftButton(button);
    } else if (button->getName() == "rightMainButton") {
        battleController->pressRightButton(button);
    } else if (button->getName() == "settings") {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

        gameStatePtr->pushSceneToStack("SettingsScene", true);
    } else if (button->getName() == "help") {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

        gameStatePtr->pushSceneToStack("HelpScene", false);
    }
}

void BattleScene::updateButtons() {
    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();
    for (auto &button : buttons) {
        switch (battleController->getState()) {
            case BattleController::rolling_heroes:
                button->setEnabled(true);

                if (button->getName() == "leftMainButton") {
                    button->setText(std::to_string(battleController->getRerolls()) + " rerolls left");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("done rolling");
                }
                break;

            case BattleController::attack_block_heroes:
                button->setEnabled(true);

                if (button->getName() == "leftMainButton") {
                    button->setText("undo");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("done attacking");
                }
                break;

            case BattleController::rolling_enemies:
            case BattleController::attack_block_enemies:
                button->setEnabled(true);

                if (button->getName() == "leftMainButton") {
                    button->setEnabled(false);
                    button->setText("enemy turn...");
                } else if (button->getName() == "rightMainButton") {
                    button->setEnabled(false);
                    button->setText("enemy turn...");
                }
                break;
            default:
                break;
        }
    }
}


void BattleScene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                         const std::unique_ptr<TextRenderer> &textRenderer) const {

    spriteRenderer->drawSprite("background_catacombs_background", 1.0f,
                               glm::vec2(0, 0), size, glm::vec3(1.0f), 0.8f);


    spriteArgs args = {};

    spriteRenderer->drawSprite(SpriteRenderer::shadow, "background_catacombs_front_pillars", 1.0f,
                               glm::vec2(-290, -180),
                               glm::vec2(700, 288), args);

    spriteRenderer->drawSprite("background_catacombs_front_pillars", 1.0f,
                               glm::vec2(-145, -180),
                               glm::vec2(700, 288),
                               glm::vec3(1.0f));

    auto &battleController = std::shared_ptr<GameStateManager>(gameState)->getBattleController();

    auto heroes = battleController->getHeroes();
    for (auto &hero : heroes) {
        if (hero->getPosition().x < 0) continue;

        hero->drawShadow(spriteRenderer, textRenderer);
        hero->draw(spriteRenderer, textRenderer);
        if (drawKeyPress) {
            hero->drawKey(spriteRenderer, textRenderer);
        }
    }

    auto enemies = battleController->getEnemies();
    for (auto &enemy : enemies) {
        if (enemy->getPosition().x < 0) continue;

        enemy->drawShadow(spriteRenderer, textRenderer);
        enemy->draw(spriteRenderer, textRenderer);
        if (drawKeyPress) {
            enemy->drawKey(spriteRenderer, textRenderer);
        }
    }

    for (auto &hero : heroes) {
        if (hero->getPosition().x < 0) continue;

        hero->drawHover(spriteRenderer, textRenderer, true);
    }

    for (auto &enemy : enemies) {
        if (enemy->getPosition().x < 0) continue;

        enemy->drawHover(spriteRenderer, textRenderer, true);
    }

    auto clickedCharacter = battleController->getClickedCharacter();
    if (clickedCharacter) {
        clickedCharacter->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

    auto clickedSpell = battleController->getClickedSpell();
    if (clickedSpell) {
        clickedSpell->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

    glm::vec2 manaPosition = glm::vec2(300, 250);
    glm::vec2 manaSize = glm::vec2(16, 16);
    glm::vec2 manaTextPosition = manaPosition + glm::vec2(9, 4);
    spriteRenderer->drawSprite("mana", 0.3f, manaPosition, manaSize,
                               glm::vec3(1.0f), 0.8f);
    textRenderer->drawText(std::to_string(battleController->getMana()) + " mana",
                           0.2f, manaTextPosition, glm::vec2(100, 1));

    renderDefaults(spriteRenderer, textRenderer);
}

}
