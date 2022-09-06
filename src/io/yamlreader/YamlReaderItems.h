//
// Created by thijs on 03-09-22.
//

#ifndef DICEGONEROGUE_YAMLREADERITEMS_H
#define DICEGONEROGUE_YAMLREADERITEMS_H

#include "YamlReader.h"

namespace DGR {

class YamlHandleItems : public YamlHandle {
    std::vector<std::shared_ptr<Item>> items = std::vector<std::shared_ptr<Item >>();

public:
    YamlHandleItems() : YamlHandle(stringCode::items) {}

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
#if DGR_DEBUG
        std::cout << "yh.items: adding object" << std::endl;
#endif

        items.push_back(std::static_pointer_cast<Item>(yamlHandle->getFeature()));
    }

    void reset() override {
        items = std::vector<std::shared_ptr<Item>>();
    };

    std::shared_ptr<void> getFeature() override {
        return std::make_shared<std::vector<std::shared_ptr<Item>>>(items);
    };
};


class YamlHandleItem : public YamlHandle {
    int level{};
    int cost{};
    Item::itemSlot slot{};
    ItemEffect effect{};
    std::string name;
public:
    explicit YamlHandleItem(std::string name, stringCode stringCode)
          : YamlHandle(stringCode), name(std::move(name)) {}

    void reset() override {
        level = {};
        cost = {};
        slot = {};
        name = "";
    };

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::item_level:
                level = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::cost:
                cost = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::item_slot:
                slot = *std::static_pointer_cast<Item::itemSlot>(yamlHandle->getFeature()).get();
                break;
            case stringCode::item_effect:
                effect = *std::static_pointer_cast<ItemEffect>(yamlHandle->getFeature()).get();
                break;
                default:
                std::cerr << "[YamlHandleItem] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    std::shared_ptr<void> getFeature() override {
        std::shared_ptr<Item> item = std::make_shared<Item>(name, cost, slot, effect, level);
        return item;
    };
};


}

#endif //DICEGONEROGUE_YAMLREADERITEMS_H
