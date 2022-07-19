//
// Created by thijs on 19-07-22.
//

#include "Inventory.h"

void DGR::Inventory::addCharacter(const std::shared_ptr<Character> &character) {
    characters.push_back(character->makeCopy(true));
}

void DGR::Inventory::setCharacters(const std::vector<std::shared_ptr<Character>> &characters_) {
    characters = {};
    for (auto &character : characters_) {
        addCharacter(character);
    }
}
