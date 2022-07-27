//
// Created by thijs on 19-07-22.
//

#include "Inventory.h"

namespace DGR {

void Inventory::addHero(const std::shared_ptr<Character> &character) {
    heroes.push_back(character->makeCopy(true));
}

void Inventory::setHeroes(const std::vector<std::shared_ptr<Character>> &characters_) {
    heroes = {};
    for (auto &character : characters_) {
        addHero(character);
    }
}

const std::vector<std::shared_ptr<Character>> &Inventory::getHeroes() const {
    return heroes;
}

std::shared_ptr<Character> Inventory::getHeroByID(int id) const {
    for (auto &hero : heroes) {
        if (hero->getUniqueID() == id) {
            return hero;
        }
    }

    return nullptr;
}

void Inventory::reset() {
    heroes = {};
}

}
