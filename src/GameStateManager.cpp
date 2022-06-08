//
// Created by thijs on 07-06-22.
//

#include "GameStateManager.h"
#include "utilities/Button.h"

namespace DGR {

GameStateManager::gameState GameStateManager::getGameState() const {
    return state;
}

Character* GameStateManager::getClickedCharacter() const {
    return clickedCharacter;
}

void GameStateManager::setClickedCharacter(Character* clickedCharacter_) {
    clickedCharacter = clickedCharacter_;
}

int GameStateManager::reroll(bool rollHeroes) {
    if (rollHeroes) {
        if (rerolls > 0) {
            rerolls--;
            for (auto &hero : heroes) {
                hero->roll();
            }
        }
#ifdef DEBUG
        else {
            std::cerr << "no rerolls remaining!" << std::endl;

        }
#endif
    } else {
        for (auto &enemy : enemies) {
            enemy->roll();
        }
    }

    return rerolls;
}

const std::vector<Hero*> &GameStateManager::getHeroes() const {
    return heroes;
}

const std::vector<Enemy*> &GameStateManager::getEnemies() const {
    return enemies;
}

void GameStateManager::setHeroes(const std::vector<Hero*> &heroes_) {
    heroes = heroes_;
}

void GameStateManager::setEnemies(const std::vector<Enemy*> &enemies_) {
    enemies = enemies_;
}

void GameStateManager::render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    if (clickedCharacter) {
        clickedCharacter->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

    glm::vec2 manaPosition = glm::vec2(288, 216);
    glm::vec2 manaSize = glm::vec2(16, 16);
    glm::vec2 manaTextPosition = manaPosition + glm::vec2(6, 4);
    spriteRenderer->drawSprite("mana", 0.3f, manaPosition, manaSize);
    textRenderer->drawText(std::to_string(mana) + "  mana", 0.2f, manaTextPosition, glm::vec2(100, 1));
}

void GameStateManager::addMana(int mana_) {
    mana += mana_;
}

bool GameStateManager::areHeroesRolling() const {
    return state == rolling_heroes;
}

bool GameStateManager::areEnemiesRolling() const {
    return state == rolling_enemies;
}

bool GameStateManager::areHeroesAttacking() const {
    return state == attack_block_heroes;
}

bool GameStateManager::areEnemiesAttacking() const {
    return state == attack_block_enemies;
}

void GameStateManager::setNextGameState() {
    switch (state) {
        case rolling_heroes:
            for (auto &hero : heroes) {
                hero->setDiceLock(false);
            }
            rerolls = rerollsMax;
            state = attack_block_heroes;
            updateButtons();
            break;
        case attack_block_heroes:
            for (auto &hero : heroes) {
                hero->setUsedDice(false);
                hero->applyDamageStep();
            }
            state = rolling_enemies;
            updateButtons();
            break;
        case rolling_enemies:
            state = attack_block_enemies;
            updateButtons();
            break;
        case attack_block_enemies:
            for (auto &enemy : enemies) {
                enemy->setUsedDice(false);
                enemy->applyDamageStep();
            }
            reroll();
            state = rolling_heroes;
            updateButtons();
            break;
    }
}

void GameStateManager::updateButtons() {
    auto buttons = window->getButtons();
    switch (state) {
        case rolling_heroes:
            for (auto &button : buttons) {
                if (button->getName() == "leftMainButton") {
                    button->setText("2 rerolls left");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("done rolling");
                }
            }
            break;

        case attack_block_heroes:
            for (auto &button : buttons) {
                if (button->getName() == "leftMainButton") {
                    button->setText("undo");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("done attacking");
                }
            }
            break;

        case rolling_enemies:
            for (auto &button : buttons) {
                if (button->getName() == "leftMainButton") {
                    button->setText("enemy turn...");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("enemy turn...");
                }
            }
            break;

        case attack_block_enemies:
            for (auto &button : buttons) {
                if (button->getName() == "leftMainButton") {
                    button->setText("enemy turn...");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("enemy turn...");
                }
            }
            break;

    }
}

Window* GameStateManager::getWindow() const {
    return window;
}

std::pair<Character*, Character*> GameStateManager::getNeighbours(Character* character) {
    std::pair<Character*, Character*> neighbours(nullptr, nullptr);
    unsigned long nHeroes = heroes.size();
    for (unsigned long i = 0; i < nHeroes; i++) {
        if (character == heroes[i]) {
            unsigned long j = i-1;
            while (j >= 0) {
                if (!heroes[j]->isDead()) {
                    std::cout << "sweep" << character->getName() << heroes[j]->getName() << std::endl;

                    neighbours.first = heroes[j];
                    break;
                }
                j--;
            }
            j = i+1;
            while (j < nHeroes) {
                if (!heroes[j]->isDead()) {
                    std::cout << "sweep" << character->getName() << heroes[j]->getName() << std::endl;

                    neighbours.second = heroes[j];
                    break;
                }
                j++;
            }
            return neighbours;
        }
    }
    unsigned long nEnemies = enemies.size();
    for (unsigned long i = 0; i < nEnemies; i++) {
        if (character == enemies[i]) {
            unsigned long j = i-1;
            while (j >= 0) {
                if (!enemies[j]->isDead()) {
                    neighbours.first = enemies[j];
                    break;
                }
                j--;
            }
            j = i+1;
            while (j < nHeroes) {
                if (!enemies[j]->isDead()) {
                    neighbours.second = enemies[j];
                    break;
                }
                j++;
            }
            return neighbours;
        }
    }
    return neighbours;
}

}