//
// Created by thijs on 22-07-22.
//

#include "Item.h"
#include "../Character.h"

namespace DGR {

std::shared_ptr<Item> Item::getSharedFromThis() {
    return shared_from_this();
}

void Item::remove() {
    auto characterPtr = std::shared_ptr<Character>(character);
    if (characterPtr) {
        characterPtr->setItem(slot, nullptr);
    }
}

void Item::add(const std::weak_ptr<Character> &character_) {
    auto characterPtr = std::shared_ptr<Character>(character_);
    characterPtr->setItem(slot, getSharedFromThis());
}

}
