//
// Created by thijs on 22-07-22.
//

#ifndef DICEGONEROGUE_ITEM_H
#define DICEGONEROGUE_ITEM_H


#include <memory>

namespace DGR {

class Character;

class ItemEffect {

};

class Item : public std::enable_shared_from_this<Item> {
private:
    std::string name;

    std::weak_ptr<Character> character{};

    int cost{};
    int value{};
    ItemEffect itemEffect{};

    bool hover = false;
public:
    Item() = default;

};

}


#endif //DICEGONEROGUE_ITEM_H
