//
// Created by thijs on 03-06-22.
//

#ifndef DICEGONEROGUE_YAMLREADER_H
#define DICEGONEROGUE_YAMLREADER_H

#include <string>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <utility>
#include <type_traits>

#include "gameobject/Hero.h"
#include "gameobject/Dice.h"

namespace DGR {

enum struct stringCode : int {
    heroes,
    hero,
    dice,
    face,
    mod,
    hp,
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
    empty
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

    [[nodiscard]] virtual void* getFeature() {
        return nullptr;
    };

    virtual void reset() {};

    virtual void handle() {};

    virtual void handle(YamlHandle* yamlHandle) {
        std::cerr << "[YamlHandle::handle(YamlHandle*)] function not implemented: "
                  << static_cast<int>(yamlHandle->getType()) << std::endl;

        exit(5);
    };

    virtual void handle(const std::string &value) {
        std::cerr << "[YamlHandle::handle(std::string)] function not implemented" << value << std::endl;
        exit(5);
    };
};

class YamlHandleHeroes : public YamlHandle {
    std::vector<Hero*>* heroes = new std::vector<Hero*>();

public:
    YamlHandleHeroes() : YamlHandle(stringCode::heroes) {}

    void handle(YamlHandle* yamlHandle) override {
#if DEBUG
        std::cout << "yh.heroes: adding object" << std::endl;
#endif
        switch (yamlHandle->getType()) {
            case stringCode::hero:
                heroes->push_back((Hero*) yamlHandle->getFeature());
                break;
            default:
                std::cerr << "[YamlHandleHeroes] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void reset() override {
        heroes = new std::vector<Hero*>();
    };

    void* getFeature() override {
        return (void*) heroes;
    };
};

class YamlHandleHero : public YamlHandle {
    Dice* dice = nullptr;
    int maxHP{};
    std::string name;
public:
    explicit YamlHandleHero(std::string name) : YamlHandle(stringCode::hero), name(std::move(name)) {}

    void reset() override {
        dice = nullptr;
        maxHP = {};
        name = "";
    };

    void handle(YamlHandle* yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::hp:
                maxHP = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::dice:
                dice = (Dice*) yamlHandle->getFeature();
                break;
            default:
                std::cerr << "[YamlHandleHero] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void* getFeature() override {
        Hero* hero = new Hero(name);
        hero->setMaxHP(maxHP);
        hero->setDice(dice);
        hero->getDice()->setHero(hero);
        hero->getDice()->setName(hero->getName());
        return (void*) hero;
    };
};

class YamlHandleDice : public YamlHandle {
    Face faces[6];

public:
    explicit YamlHandleDice() : YamlHandle(stringCode::dice) {};

    void reset() override {
        for (auto &face : faces) {
            face = {};
        }
    }

    void handle(YamlHandle* yamlHandle) override {
        Face* face;
        int index;

        switch (yamlHandle->getType()) {
            case stringCode::face:
                face = (Face*) yamlHandle->getFeature();
                index = face->getFace_();
                faces[index] = *face;
                break;
            default:
                std::cerr << "[YamlHandleDice] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void* getFeature() override {
        Dice* dice = new Dice();
        for (int i = 0; i < 6; i++) {
            dice->setFace(faces[i], i);
            dice->getFace(i)->setDice(dice);
            dice->getFace(i)->setName(dice->getName());
        }
        return (void*) dice;
    };
};

class YamlHandleFace : public YamlHandle {
    int face_{};
    int value{};
    faceType type{};
    FaceModifier modifiers{};
public:
    explicit YamlHandleFace(int face_) : YamlHandle(stringCode::face), face_(face_) {};

    void reset() override {
        face_ = {};
        value = {};
        type = {};
        modifiers = {};
    }

    void handle(YamlHandle* yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::damage:
                type = faceType::damage;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::shield:
                type = faceType::shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::heal:
                type = faceType::heal;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::mana:
                type = faceType::mana;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::shield_and_mana:
                type = faceType::shield_and_mana;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::heal_and_shield:
                type = faceType::heal_and_shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::damage_and_self_shield:
                type = faceType::damage_and_self_shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::empty:
                type = faceType::empty;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::mod:
                modifiers.addModifier(*(std::string*) yamlHandle->getFeature());
                break;
            default:
                std::cerr << "[YamlHandleFace] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void* getFeature() override {
        Face* face = new Face(face_, value, type, modifiers);
        return (void*) face;
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

    void* getFeature() override {
        return (void*) &feature;
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

    void* getFeature() override {
        return (void*) &feature;
    };
};

class YamlReader {
private:
    YamlHandle* handle = nullptr;

    std::map<std::string, YamlHandle*> keyToFunc;

public:
    YamlReader();

    void readFile(const std::string &string);

    YamlHandle* getData() {
        return handle;
    };
};

}

#endif //DICEGONEROGUE_YAMLREADER_H
