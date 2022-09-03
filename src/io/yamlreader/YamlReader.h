//
// Created by thijs on 03-06-22.
//

#ifndef DICEGONEROGUE_YAMLREADER_H
#define DICEGONEROGUE_YAMLREADER_H

#include <memory>
#include <utility>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <type_traits>

#include "gameobject/Character.h"
#include "gameobject/dice/Dice.h"
#include "gameobject/dice/Face.h"
#include "gameobject/spell/Spell.h"

namespace DGR {

enum struct stringCode : int {
    characters,
    name,
    hero,
    cost,
    enemy,
    dice,
    face,
    mod,
    hp,
    size,
    damage,
    mana,
    heal,
    shield,
    dodge,
    undying,
    heal_and_shield,
    heal_and_mana,
    shield_and_mana,
    damage_and_mana,
    damage_and_self_shield,
    spell,
    damage_or_shield,
    heal_or_shield,
    damage_if_full_health,
    kill_if_below_threshold,
    bonus_health,
    cleanse,
    face_bonus,

    items,
    item,

    empty,
};

class YamlHandle {
protected:
    stringCode type;

public:
    explicit YamlHandle(stringCode type) : type(type) {};

    virtual ~YamlHandle() = default;

    [[nodiscard]] const stringCode &getType() const {
        return type;
    }

    [[nodiscard]] virtual std::shared_ptr<void> getFeature() {
        return nullptr;
    };

    virtual void reset() {};

    virtual void handle() {};

    virtual void handle(std::shared_ptr<YamlHandle> yamlHandle) {
        std::cerr << "[YamlHandle::handle( std::shared_ptr<YamlHandle>)] function not implemented: "
                  << static_cast<int>(yamlHandle->getType()) << std::endl;

        exit(5);
    };

    virtual void handle(const std::string &value) {
        std::cerr << "[YamlHandle::handle(std::string)] function not implemented" << value << std::endl;
        exit(5);
    };
};

class YamlHandleString : public YamlHandle {
    std::string feature;
public:
    explicit YamlHandleString(stringCode stringCode) : YamlHandle(stringCode) {};

    void handle(const std::string &value) override {
        if (value[0] == '\"' && value[value.size() - 1] == '\"') {
            feature = value.substr(1, value.size() - 2);
        } else {
            std::cerr << "[YamlHandleString] please enter a string";
            exit(4);
        }
    }

    std::shared_ptr<void> getFeature() override {
        return std::make_shared<std::string>(feature);
    }
};

class YamlHandleInt : public YamlHandle {
    int feature{};
public:
    explicit YamlHandleInt(stringCode stringCode) : YamlHandle(stringCode) {};

    void handle(const std::string &value) override {
        char** ptr = nullptr;
        feature = (int) strtol(value.c_str(), ptr, 10);
    };

    std::shared_ptr<void> getFeature() override {
        return std::make_shared<int>(feature);
    };
};

class YamlReader {
private:
    std::shared_ptr<YamlHandle> handle = nullptr;

    std::map<std::string, std::shared_ptr<YamlHandle>> keyToFunc;

public:
    YamlReader();

    void readFile(const std::string &string);

    std::shared_ptr<YamlHandle> getData() {
        return handle;
    };

};

}

#endif //DICEGONEROGUE_YAMLREADER_H
