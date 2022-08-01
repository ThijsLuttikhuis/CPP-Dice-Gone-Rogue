//
// Created by thijs on 19-07-22.
//

#ifndef DICEGONEROGUE_INVENTORY_H
#define DICEGONEROGUE_INVENTORY_H


#include <vector>
#include <memory>
#include "gameobject/Character.h"

namespace DGR {

class Inventory {
private:
    std::vector<std::shared_ptr<Character>> heroes = {};

    //std::vector<std::shared_ptr<Item>> items;
public:
    Inventory() = default;

    void addHero(const std::shared_ptr<Character> &character);

    void setHeroes(const std::vector<std::shared_ptr<Character>> &characters_);

    const std::vector<std::shared_ptr<Character>> &getHeroes();

};

}

#endif //DICEGONEROGUE_INVENTORY_H
