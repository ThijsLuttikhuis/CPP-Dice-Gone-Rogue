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
public:
    enum itemSlot {
        head,
        chest,
        legs,
        feet,
        left_ring,
        right_ring,
        any_ring
    };
private:

    std::string name;

    std::weak_ptr<Character> character{};

    int cost{};
    itemSlot slot{};
    ItemEffect itemEffect{};
    int itemLevel{};

    bool hover = false;
public:
    Item() = default;

    /// getters
    std::shared_ptr<Item> getSharedFromThis();

    void remove();

    void add(const std::weak_ptr<Character> &character_);

};

}


#endif //DICEGONEROGUE_ITEM_H
