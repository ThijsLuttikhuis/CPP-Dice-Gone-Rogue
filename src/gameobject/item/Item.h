//
// Created by thijs on 22-07-22.
//

#ifndef DICEGONEROGUE_ITEM_H
#define DICEGONEROGUE_ITEM_H


#include <memory>
#include <utility>
#include <ui/Button.h>

namespace DGR {

class Character;

class ItemEffect {

};

class Item : public std::enable_shared_from_this<Item>, public Button {
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

    std::weak_ptr<Character> character{};

    int cost{};
    itemSlot slot{};
    ItemEffect itemEffect{};
    int itemLevel{};

    bool hover = false;
public:
    Item() : Button("", glm::vec2(), glm::vec2()) {};

    Item(std::string name, int cost, itemSlot slot, ItemEffect itemEffect, int itemLevel)
          : Button(std::move(name), glm::vec2(), glm::vec2(16.0f, 16.0f)),
            cost(cost), slot(slot), itemEffect(itemEffect), itemLevel(itemLevel) {}

    /// getters
    std::shared_ptr<Item> getSharedFromThis();

    void remove();

    void add(const std::weak_ptr<Character> &character_);

};

}


#endif //DICEGONEROGUE_ITEM_H
