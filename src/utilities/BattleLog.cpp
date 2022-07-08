//
// Created by thijs on 11-06-22.
//

#include <iostream>
#include <utility>

#include "scene/BattleScene.h"
#include "BattleLog.h"
#include "GameStateManager.h"

namespace DGR {

BattleLog::BattleLog(std::weak_ptr<BattleScene> battleScene)
      : battleScene(std::move(battleScene)) {}

std::shared_ptr<Character> BattleLog::getStoredCharacter(int id) {
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

std::shared_ptr<Spell> BattleLog::getStoredSpell(int id) {
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

void BattleLog::addAttack(Character* character, Character* otherCharacter) {
    thisTurn.attackOrder.push_back(attackType::character);

    int characterID = character->getUniqueID();
    int otherCharacterID = otherCharacter ? otherCharacter->getUniqueID() : -1;

    thisTurn.attackOrderIDs.emplace_back(characterID, otherCharacterID);
}

void BattleLog::addAttack(Spell* spell, Character* character) {
    thisTurn.attackOrder.push_back(attackType::spell);

    int characterID = character->getUniqueID();
    int spellID = std::shared_ptr<Character>(spell->getCharacter())->getUniqueID();

    thisTurn.attackOrderIDs.emplace_back(characterID, spellID);
}

void BattleLog::undo() {
    if (thisTurn.attackOrder.empty()) {
        return;
    }
    auto battleScenePtr = std::shared_ptr<BattleScene>(battleScene);
    battleScenePtr->setMana(mana);

    // create new copies
    std::vector<std::shared_ptr<Character>> heroes_ = {};
    std::vector<std::shared_ptr<Character>> enemies_ = {};
    for (auto &hero : heroes) {
        std::shared_ptr<Character> copy = hero->makeCopy(true);
        heroes_.push_back(copy);
    }
    for (auto &enemy : enemies) {
        std::shared_ptr<Character> copy = enemy->makeCopy(true);
        enemies_.push_back(copy);
    }

    battleScenePtr->setHeroes(heroes);
    battleScenePtr->setEnemies(enemies);

    // apply attacks until index-1
    int undoTillIndex = (int) thisTurn.attackOrder.size() - 1;
    for (int i = 0; i < undoTillIndex; i++) {
        if (thisTurn.attackOrder[i] == attackType::character) {
            auto ids = thisTurn.attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto otherCharacter = ids.second >= 0 ? getStoredCharacter(ids.second) : nullptr;
            auto success = character->interact(otherCharacter, battleScenePtr, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(character) unsuccessful!" << std::endl;
                exit(-10);
            }
            if (otherCharacter) {
                otherCharacter->setUsedDice(true);
            } else {
                character->setUsedDice(true);
            }
        } else if (thisTurn.attackOrder[i] == attackType::spell) {
            auto ids = thisTurn.attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto spell = ids.second >= 0 ? getStoredSpell(ids.second) : nullptr;
            auto success = character->interact(spell, battleScenePtr, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(spell) unsuccessful!" << std::endl;
                exit(-10);
            }
            battleScenePtr->addMana(-spell->getCost());
        }
    }

    thisTurn.attackOrder.pop_back();
    thisTurn.attackOrderIDs.pop_back();

    std::swap(heroes, heroes_);
    std::swap(enemies, enemies_);
}

void BattleLog::setState(const std::vector<std::shared_ptr<Character>> &heroes_,
                         const std::vector<std::shared_ptr<Character>> &enemies_, int mana_) {
    thisTurn.attackOrder = {};
    thisTurn.attackOrderIDs = {};
    mana = mana_;

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

void BattleLog::reset() {
    setState({}, {}, 0);

    thisTurn.attackOrder = {};
    thisTurn.attackOrderIDs = {};
}

void BattleLog::saveTurn() {
    turns.push_back(thisTurn);
    printCharacterIDs();
    thisTurn.toString();
}

void BattleLog::printCharacterIDs() {
    std::string characterIdString;
    for (const auto &hero : heroes) {
        characterIdString += "id " + std::to_string(hero->getUniqueID()) + ": " + hero->getName() + ", " +
                             hero->getDice()->getCurrentFace()->getToolTipString() + ";\n";
    }
    for (const auto &enemy : enemies) {
        characterIdString += "id " + std::to_string(enemy->getUniqueID()) + ": " + enemy->getName() + ", " +
                             enemy->getDice()->getCurrentFace()->getToolTipString() + ";\n";
    }
    std::cout << characterIdString << std::endl;
}

}
