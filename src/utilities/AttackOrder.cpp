//
// Created by thijs on 11-06-22.
//

#include <iostream>

#include "scene/BattleScene.h"
#include "AttackOrder.h"
#include "GameStateManager.h"

namespace DGR {

std::shared_ptr<Character> AttackOrder::getStoredCharacter(int id) {
    for (auto &hero : heroes) {
        if (hero->getUniqueID() == id) {
            return hero;
        }
    }
    for (auto &enemy : enemies) {
        if (enemy->getUniqueID() == id) {
            return enemy;
        }
    }

    std::cerr << "AttackOrder::getStoredCharacter: error: no character found!" << std::endl;
    exit(404);
}

std::shared_ptr<Spell> AttackOrder::getStoredSpell(int id) {
    for (auto &hero : heroes) {
        if (hero->getUniqueID() == id) {
            return hero->getSpell();
        }
    }
    for (auto &enemy : enemies) {
        if (enemy->getUniqueID() == id) {
            return enemy->getSpell();
        }
    }

    std::cerr << "AttackOrder::getStoredSpell: error: no spell found!" << std::endl;
    exit(404);
}

void AttackOrder::addAttack(std::shared_ptr<Character> character, std::shared_ptr<Character> otherCharacter) {
    attackOrder.push_back(attackType::character);

    int characterID = character->getUniqueID();
    int otherCharacterID = otherCharacter ? otherCharacter->getUniqueID() : -1;

    attackOrderIDs.emplace_back(characterID, otherCharacterID);
}

void AttackOrder::addAttack(std::shared_ptr<Spell> spell, std::shared_ptr<Character> character) {
    attackOrder.push_back(attackType::spell);

    int characterID = character->getUniqueID();
    int spellID = spell->getCharacter()->getUniqueID();

    attackOrderIDs.emplace_back(characterID, spellID);
}

void AttackOrder::undo() {
    if (attackOrder.empty()) {
        return;
    }

    battleScene->setMana(mana);

    // create new copies
    std::vector<Character*> heroes_ = {};
    std::vector<Character*> enemies_ = {};
    for (auto &hero : heroes) {
        std::shared_ptr<Character> copy = hero->makeCopy(true);
        heroes_.push_back(copy);
    }
    for (auto &enemy : enemies) {
        std::shared_ptr<Character> copy = enemy->makeCopy(true);
        enemies_.push_back(copy);
    }

    battleScene->setHeroes(heroes);
    battleScene->setEnemies(enemies);

    // apply attacks until index-1
    int undoTillIndex = (int) attackOrder.size() - 1;
    for (int i = 0; i < undoTillIndex; i++) {
        if (attackOrder[i] == attackType::character) {
            auto ids = attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto otherCharacter = ids.second >= 0 ? getStoredCharacter(ids.second) : nullptr;
            auto success = character->interact(otherCharacter, battleScene, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(character) unsuccessful!" << std::endl;
                exit(-10);
            }
            if (otherCharacter) {
                otherCharacter->setUsedDice(true);
            } else {
                character->setUsedDice(true);
            }
        } else if (attackOrder[i] == attackType::spell) {
            auto ids = attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto spell = ids.second >= 0 ? getStoredSpell(ids.second) : nullptr;
            auto success = character->interact(spell, battleScene, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(spell) unsuccessful!" << std::endl;
                exit(-10);
            }
            battleScene->addMana(-spell->getCost());

        }
    }

    attackOrder.pop_back();
    attackOrderIDs.pop_back();

    std::swap(heroes, heroes_);
    std::swap(enemies, enemies_);

}

void AttackOrder::setState(const std::vector<std::shared_ptr<Character>> &heroes_,
                           const std::vector<std::shared_ptr<Character>> &enemies_, int mana_) {
    attackOrder = {};
    attackOrderIDs = {};
    mana = mana_;

    // delete previous copies
    for (auto &hero : heroes) {
        delete hero;
    }
    for (auto &enemy : enemies) {
        delete enemy;
    }
    heroes = {};
    enemies = {};

    // set new copies
    for (auto &hero : heroes_) {
        std::shared_ptr<Character> copy = hero->makeCopy(true);
        heroes.push_back(copy);
    }
    for (auto &enemy : enemies_) {
        std::shared_ptr<Character> copy = enemy->makeCopy(true);
        enemies.push_back(copy);
    }
}

void AttackOrder::reset() {
    setState({}, {}, 0);

    attackOrder = {};
    attackOrderIDs = {};
}

}
