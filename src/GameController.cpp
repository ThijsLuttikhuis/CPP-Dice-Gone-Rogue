//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "shaders/TextRenderer.h"
#include "utilities/Random.h"
#include "GameController.h"
#include "ui/Window.h"
#include "iofilemanager/YamlReader.h"
#include "GameStateManager.h"
#include "ui/Button.h"
#include "gameobject/spell/Spell.h"
#include "gameobject/Character.h"

namespace DGR {

GameController::GameController(Window* window) {
    gameState = new GameStateManager(window);

    auto* shader = new Shader();
    shader->compile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

    glm::mat4 projection = glm::ortho(0.0f, (float) window->getWidth(),
                                      (float) window->getHeight(), 0.0f,
                                      -1.0f, 1.0f);

    textRenderer = new TextRenderer(shader, projection);
    spriteRenderer = new SpriteRenderer(shader, projection);
    spriteRenderer->addAllTexturesInDir("textures");

    YamlReader yamlReaderHeroes;
    yamlReaderHeroes.readFile("heroes");
    auto heroes = *(std::vector<Character*>*) yamlReaderHeroes.getData()->getFeature();
    gameState->setHeroes(heroes);

    YamlReader yamlReaderEnemies;
    yamlReaderEnemies.readFile("enemies");
    auto enemies = *(std::vector<Character*>*) yamlReaderEnemies.getData()->getFeature();
    gameState->setEnemies(enemies);

    gameState->reroll();

    auto attackOrder = new AttackOrder(gameState);
    gameState->setAttackOrder(attackOrder);
}

const std::vector<Character*> &GameController::getHeroes() const {
    return gameState->getHeroes();
}

const std::vector<Character*> &GameController::getEnemies() const {
    return gameState->getEnemies();
}

SpriteRenderer* GameController::getSpriteRenderer() const {
    return spriteRenderer;
}

TextRenderer* GameController::getTextRenderer() const {
    return textRenderer;
}

void GameController::update() {

    glfwPollEvents();
    t = glfwGetTime();

    dt = t - tPrev;
    tPrev = t;

    alignCharacterPositions(dt);

    //TODO: add animations etc
    int slowDown = 30;
    if (gameState->areEnemiesRolling()) {
        animationCounter++;
        if (animationCounter % slowDown == 0) {
            gameState->reroll();
            for (auto &enemy : getEnemies()) {
                auto currentFace = enemy->getDice()->getCurrentFace()->getFace_();
                if (currentFace == 0 || currentFace == 2) {
                    enemy->setDiceLock(true);
                }
            }
        }
        if (gameState->getRerolls() == 0) {
            gameState->setNextGameState();
            animationCounter = 0;
        }
    } else if (gameState->areEnemiesAttacking()) {
        if (animationCounter < (int) getEnemies().size() * slowDown) {
            if ((animationCounter++ % slowDown) == 0) {
                int i = animationCounter / slowDown;
                enemyAttack(i);
            }
        } else {
            animationCounter = 0;
            gameState->setNextGameState();
        }
    }
}

void GameController::alignCharacterPositions(double dt) {
    const int width = gameState->getWindow()->getWidth();
    const int center = width / 2;
    const float moveSpeed = 100.0f;
    const float maxDPos = moveSpeed * (float) dt;
    const int dWidth = 16;
    const int maxTotalWidth = 6 * (dWidth + 32);

    int startLeft;
    int totalWidth;
    int left, up;
    bool isHeroes = false;
    for (auto &characters : {gameState->getHeroes(), gameState->getEnemies()}) {
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
        up = isHeroes ? 13 * 16 : 6 * 16;

        for (auto &character : characters) {
            if (!character->isDead()) {
                glm::vec2 size = character->getSize();
                if ((int)size.x + left - startLeft > maxTotalWidth) {
                    std::cout << left << startLeft << maxTotalWidth << std::endl;
                    std::cout << "too many characters" << std::endl;
                    character->setPosition(-left, -up);
                    continue;
                }
                glm::vec2 targetPos(left, (float)up - size.y);
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

void GameController::clickSpell(Spell* spell) {
    switch (gameState->getGameState()) {
        case GameStateManager::rolling_heroes:
            break;
        case GameStateManager::attack_block_heroes:
            if (gameState->getClickedSpell() == spell) {
                gameState->setClickedSpell(nullptr);
            } else {
                std::cout << gameState->getMana() << ", " << spell->getCost() << std::endl;
                if (gameState->getMana() >= spell->getCost()) {
                    std::cout << "done" << std::endl;
                    std::cout << gameState->getMana() << ", " << spell->getCost() << std::endl;
                    gameState->setClickedSpell(spell);
                    gameState->setClickedCharacter(nullptr);
                }
            }
            break;
        case GameStateManager::rolling_enemies:
            break;
        case GameStateManager::attack_block_enemies:
            break;
    }
}

void GameController::clickCharacter(Character* character) {
    bool success;
    Character* clickedCharacter;
    Spell* clickedSpell;
    switch (gameState->getGameState()) {
        case GameStateManager::rolling_heroes:
            if (character->getCharacterType() == "hero") {
                character->toggleDiceLock();
            }
            break;
        case GameStateManager::rolling_enemies:
            break;
        case GameStateManager::attack_block_heroes:
            clickedSpell = gameState->getClickedSpell();
            clickedCharacter = gameState->getClickedCharacter();
            if (clickedCharacter) {
                success = character->interact(clickedCharacter, gameState);
                if (success) {
                    clickedCharacter->setUsedDice(true);
                }
                gameState->setClickedCharacter(nullptr);
                break;
            }
            if (clickedSpell) {
                success = character->interact(clickedSpell, gameState);
                if (success) {
                    gameState->addMana(- clickedSpell->getCost());
                }
                gameState->setClickedSpell(nullptr);
                break;
            }
            if (character->getCharacterType() == "hero") {
                if (!character->getUsedDice()) {
                    success = character->interact((Character*) nullptr, gameState);
                    if (success) {
                        character->setUsedDice(true);
                    } else {
                        gameState->setClickedCharacter(character);
                        gameState->setClickedSpell(nullptr);
                    }
                }
                break;
            }
            break;
        case GameStateManager::attack_block_enemies:
            break;
    }
}

void GameController::pressButton(Button* button) {
    std::cout << "pressed a button!" << std::endl;

    if (button->getName() == "leftMainButton") {
        if (gameState->areHeroesRolling()) {
            int rerollsLeft = gameState->reroll();
            button->setText(std::to_string(rerollsLeft) + " rerolls left");
            if (rerollsLeft == 0) {
                gameState->setNextGameState();
            }
        } else if (gameState->areHeroesAttacking()) {
            gameState->getAttackOrder()->undo();
        }
    }

    if (button->getName() == "rightMainButton") {
        if (gameState->areHeroesRolling()) {
            gameState->setNextGameState();
        } else if (gameState->areHeroesAttacking()) {
            gameState->setNextGameState();
        }
    }
}

void GameController::enemiesAttack() {
    for (int i = 0; i < (int) gameState->getEnemies().size(); i++) {
        enemyAttack(i);
    }
}

void GameController::enemyAttack(int index) {
    auto heroes = gameState->getHeroes();
    int nHeroes = heroes.size();

    auto enemies = gameState->getEnemies();
    int nEnemies = enemies.size();

    auto enemy = enemies[index];
    bool success;

    if (enemy->isDead()) {
        return;
    }

    // check if you can interact with yourself
    success = enemy->interact((Character*) nullptr, gameState);

    // check if you can interact with an ally (=enemy from their perspective)
    if (!success) {
        int mostIncomingDamage = 0;
        for (auto &otherEnemy : gameState->getEnemies()) {
            mostIncomingDamage = std::max(mostIncomingDamage, otherEnemy->getIncomingDamage());
        }
        if (mostIncomingDamage > 0) {
            for (auto &otherEnemy : enemies) {
                if (otherEnemy->getIncomingDamage() == mostIncomingDamage) {
                    success = otherEnemy->interact(enemy, gameState);
                }
            }
        } else {
            int rng = Random::randInt(0, nEnemies - 1);
            success = enemies[rng]->interact(enemy, gameState);
        }
    }

    // finally, check if you can interact with an enemy (=hero from their perspective)
    if (!success) {
        int rng;
        while (true) {
            rng = Random::randInt(0, nHeroes - 1);
            if (!heroes[rng]->isDead()) {
                success = heroes[rng]->interact(enemy, gameState);
                break;
            }
        }
    }

    if (!success) {
        std::cerr << "GameController::enemiesAttack(): ERROR in attack logic!" << std::endl;
    }
}

void GameController::render() {
    for (auto &hero : gameState->getHeroes()) {
        hero->draw(spriteRenderer, textRenderer);
    }

    for (auto &enemy : gameState->getEnemies()) {
        enemy->draw(spriteRenderer, textRenderer);
    }

    for (auto &hero : gameState->getHeroes()) {
        hero->drawHover(spriteRenderer, textRenderer);
    }

    for (auto &enemy : gameState->getEnemies()) {
        enemy->drawHover(spriteRenderer, textRenderer);
    }

    gameState->render(spriteRenderer, textRenderer);
}


}
