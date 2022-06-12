//
// Created by thijs on 11-06-22.
//

#include <iostream>

#include "AttackOrder.h"
#include "GameStateManager.h"

namespace DGR {

Character* AttackOrder::getStoredCharacter(int id) {
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
}

Spell* AttackOrder::getStoredSpell(int id) {
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
}

void AttackOrder::addAttack(Character* character, Character* otherCharacter) {
    attackOrder.push_back(attackType::character);

    int characterID = character->getUniqueID();
    int otherCharacterID = otherCharacter ? otherCharacter->getUniqueID() : -1;

    attackOrderIDs.emplace_back(characterID, otherCharacterID);
}

void AttackOrder::addAttack(Spell* spell, Character* character) {
    attackOrder.push_back(attackType::spell);

    int characterID = character->getUniqueID();
    int spellID = spell->getCharacter()->getUniqueID();

    attackOrderIDs.emplace_back(characterID, spellID);
}

void AttackOrder::undo() {
    if (attackOrder.empty()) {
        return;
    }

    gameState->setMana(mana);

    // create new copies
    std::vector<Hero*> heroes_ = {};
    std::vector<Enemy*> enemies_ = {};
    for (auto &hero : heroes) {
        Hero* copy = hero->makeCopy(true);
        heroes_.push_back(copy);
    }
    for (auto &enemy : enemies) {
        Enemy* copy = enemy->makeCopy(true);
        enemies_.push_back(copy);
    }

    // apply attacks until index-1
    int undoTillIndex = (int)attackOrder.size() - 1;
    for (int i = 0; i < undoTillIndex; i++) {
        if (attackOrder[i] == attackType::character) {
            auto ids = attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto otherCharacter = ids.second >= 0 ? getStoredCharacter(ids.second) : nullptr;
            auto success = character->interact(otherCharacter, gameState, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(character) unsuccessful!" << std::endl;
                exit(-10);
            }
            if (otherCharacter) {
                otherCharacter->setUsedDice(true);
            }
            else {
                character->setUsedDice(true);
            }
        }
        else if (attackOrder[i] == attackType::spell) {
            auto ids = attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto spell = ids.second >= 0 ? getStoredSpell(ids.second) : nullptr;
            auto success = character->interact(spell, gameState, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(spell) unsuccessful!" << std::endl;
                exit(-10);
            }
            gameState->addMana(- spell->getCost());

        }
    }

    attackOrder.pop_back();
    attackOrderIDs.pop_back();

    std::swap(heroes, heroes_);
    std::swap(enemies, enemies_);

    gameState->setHeroes(heroes_);
    gameState->setEnemies(enemies_);
}

void AttackOrder::setState(const std::vector<Hero*> &heroes_, const std::vector<Enemy*> &enemies_, int mana_) {

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
        Hero* copy = hero->makeCopy(true);
        heroes.push_back(copy);
    }
    for (auto &enemy : enemies_) {
        Enemy* copy = enemy->makeCopy(true);
        enemies.push_back(copy);
    }
}


}