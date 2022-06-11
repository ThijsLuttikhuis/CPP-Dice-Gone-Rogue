//
// Created by thijs on 11-06-22.
//

#include <iostream>

#include "AttackOrder.h"
#include "GameStateManager.h"

namespace DGR {

void AttackOrder::addAttack(Character* character, Character* otherCharacter) {
    attackOrder.push_back(attackType::character);
    characterAttacks.push_back({character, otherCharacter});
}

void AttackOrder::addAttack(Spell* spell, Character* character) {
    attackOrder.push_back(attackType::spell);
    characterSpells.push_back({spell, character});
}

void AttackOrder::undo() {

    std::vector<Hero*> heroes_ = {};
    std::vector<Enemy*> enemies_ = {};

    // set new copies
    for (auto &hero : heroes) {
        Hero* copy = hero->makeCopy();
        heroes_.push_back(copy);
    }
    for (auto &enemy : enemies) {
        Enemy* copy = enemy->makeCopy();
        enemies_.push_back(copy);
    }

    gameState->setHeroes(heroes_);
    gameState->setEnemies(enemies_);

    setCharacters(heroes_, enemies_);
}

void AttackOrder::setCharacters(const std::vector<Hero*> &heroes_, const std::vector<Enemy*> &enemies_) {
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
        Hero* copy = hero->makeCopy();
        heroes.push_back(copy);
    }
    for (auto &enemy : enemies_) {
        Enemy* copy = enemy->makeCopy();
        enemies.push_back(copy);
    }
}


}