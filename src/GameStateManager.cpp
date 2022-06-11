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

Spell* GameStateManager::getClickedSpell() const {
    return clickedSpell;
}

int GameStateManager::getRerolls() const {
    return rerolls;
}

int GameStateManager::getMana() const {
    return mana;
}

AttackOrder* GameStateManager::getAttackOrder() const {
    return attackOrder;
}

void GameStateManager::setAttackOrder(AttackOrder* attackOrder_) {
    attackOrder = attackOrder_;
}

void GameStateManager::setClickedCharacter(Character* clickedCharacter_) {
    clickedCharacter = clickedCharacter_;
}

void GameStateManager::setClickedSpell(Spell* clickedSpell_) {
    clickedSpell = clickedSpell_;
}

int GameStateManager::reroll() {

    if (rerolls > 0) {
        rerolls--;

        if (areHeroesRolling()) {
            for (auto &hero : heroes) {
                hero->roll();
            }
        } else if (areEnemiesRolling()) {
            for (auto &enemy : enemies) {
                enemy->roll();
            }
        }
        else {
            std::cerr << "GameStateManager::reroll: error, not in a rolling phase!" << std::endl;
        }
    }
#ifdef DEBUG
    else {
    std::cerr << "GameStateManager::reroll: no rerolls remaining!" << std::endl;
    }
#endif
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

    if (clickedSpell) {
        clickedSpell->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
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
            attackOrder->setCharacters(heroes, enemies);
            attackOrder->setCharacters(heroes, enemies);

            state = attack_block_heroes;
            updateButtons();
            break;
        case attack_block_heroes:
            for (auto &hero : heroes) {
                hero->setUsedDice(false);
                hero->applyDamageStep();
            }
            state = rolling_enemies;
            reroll();
            updateButtons();
            break;
        case rolling_enemies:
            for (auto &enemy : enemies) {
                enemy->setDiceLock(false);
            }
            rerolls = rerollsMax;
            state = attack_block_enemies;
            updateButtons();
            break;
        case attack_block_enemies:
            for (auto &enemy : enemies) {
                enemy->setUsedDice(false);
                enemy->applyDamageStep();
            }
            state = rolling_heroes;
            reroll();
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
                    button->setText(std::to_string(getRerolls()) + " rerolls left");
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
    int nHeroes = heroes.size();
    for (int i = 0; i < nHeroes; i++) {
        if (character == heroes[i]) {
            int j = i - 1;
            while (j >= 0) {
                if (!heroes[j]->isDead()) {
                    std::cout << "sweep" << character->getName() << heroes[j]->getName() << std::endl;

                    neighbours.first = heroes[j];
                    break;
                }
                j--;
            }
            j = i + 1;
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
    int nEnemies = enemies.size();
    for (int i = 0; i < nEnemies; i++) {
        if (character == enemies[i]) {
            int j = i - 1;
            while (j >= 0) {
                if (!enemies[j]->isDead()) {
                    neighbours.first = enemies[j];
                    break;
                }
                j--;
            }
            j = i + 1;
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
