//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "gameobject/Hero.h"
#include "shaders/TextRenderer.h"
#include "utilities/Random.h"
#include "GameController.h"
#include "utilities/Window.h"
#include "iofilemanager/YamlReader.h"
#include "GameStateManager.h"
#include "utilities/Button.h"
#include "gameobject/spell/Spell.h"

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
    auto heroes = *(std::vector<Hero*>*) yamlReaderHeroes.getData()->getFeature();

    int x = 0;
    for (auto &hero : heroes) {
        glm::vec2 pos(2 * x++ * 24 + 16, 5 * 32);
        glm::vec2 size(32, 32);

        hero->setPosition(pos);
        hero->setSize(size);
    }

    gameState->setHeroes(heroes);

    YamlReader yamlReaderEnemies;
    yamlReaderEnemies.readFile("enemies");
    auto enemies = *(std::vector<Enemy*>*) yamlReaderEnemies.getData()->getFeature();

    x = 0;
    for (auto &enemy : enemies) {
        glm::vec2 pos(2 * x++ * 24 + 16, 2 * 32);
        glm::vec2 size(16, 16);

        enemy->setPosition(pos);
        enemy->setSize(size);
    }

    gameState->setEnemies(enemies);

    gameState->reroll();

    auto attackOrder = new AttackOrder(gameState);
    gameState->setAttackOrder(attackOrder);
}

const std::vector<Hero*> &GameController::getHeroes() const {
    return gameState->getHeroes();
}

const std::vector<Enemy*> &GameController::getEnemies() const {
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

    int totalWidth;
    int left, up;

    // heroes
    totalWidth = 0;
    for (auto &hero : gameState->getHeroes()) {
        if (!hero->isDead()) {
            totalWidth += dWidth + (int) hero->getSize().x;
        }
    }

    left = (int) (center * 0.7) - totalWidth / 2;
    up = 5 * 32;
    for (auto &hero : gameState->getHeroes()) {
        if (!hero->isDead()) {
            glm::vec2 targetPos(left, up);
            glm::vec2 pos = hero->getPosition();
            glm::vec2 dPos(targetPos - pos);

            if (glm::length(dPos) > maxDPos && glm::length(dPos) < moveSpeed) {
                dPos = glm::normalize(dPos) * maxDPos;
                hero->setPosition(pos + dPos);
            } else {
                hero->setPosition(targetPos);
            }
            left += dWidth + (int) hero->getSize().x;
        } else {
            hero->setPosition(-left, -up);
        }
    }

    // enemies
    totalWidth = 0;
    for (auto &enemy : gameState->getEnemies()) {
        if (!enemy->isDead()) {
            totalWidth += dWidth + (int) enemy->getSize().x;
        }
    }

    left = (int) (center / 0.7) - totalWidth / 2;
    up = 2 * 32;
    for (auto &enemy : gameState->getEnemies()) {
        if (!enemy->isDead()) {
            auto targetPos = glm::vec2(left, up);
            auto pos = enemy->getPosition();
            if (pos != targetPos) {
                auto dPos = glm::vec2(targetPos - pos);
                if (glm::length(dPos) > maxDPos && glm::length(dPos) < moveSpeed) {
                    dPos = glm::normalize(dPos) * maxDPos;
                }
                enemy->setPosition(pos + dPos);
            }
            left += dWidth + (int) enemy->getSize().x;
        } else {
            enemy->setPosition(-left, -up);
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
