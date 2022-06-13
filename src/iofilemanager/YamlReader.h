//
// Created by thijs on 03-06-22.
//

#ifndef DICEGONEROGUE_YAMLREADER_H
#define DICEGONEROGUE_YAMLREADER_H

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

class YamlHandleCharacters : public YamlHandle {
    std::vector<Character*>* characters = new std::vector<Character*>();

public:
    YamlHandleCharacters() : YamlHandle(stringCode::characters) {}

    void handle(YamlHandle* yamlHandle) override {
#if DEBUG
        std::cout << "yh.characters: adding object" << std::endl;
#endif
        switch (yamlHandle->getType()) {
            case stringCode::hero:
                characters->push_back((Character*) yamlHandle->getFeature());
                break;
            case stringCode::enemy:
                characters->push_back((Character*) yamlHandle->getFeature());
                break;
            default:
                std::cerr << "[YamlHandleCharacters] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void reset() override {
        characters = new std::vector<Character*>();
    };

    void* getFeature() override {
        return (void*) characters;
    };
};

class YamlHandleCharacter : public YamlHandle {
    Spell* spell = nullptr;
    Dice* dice = nullptr;
    int maxHP{};
    std::string name;
public:
    explicit YamlHandleCharacter(std::string name, stringCode stringCode)
          : YamlHandle(stringCode), name(std::move(name)) {}

    void reset() override {
        spell = nullptr;
        dice = nullptr;
        maxHP = {};
    };

    void handle(YamlHandle* yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::hp:
                maxHP = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::spell:
                spell = (Spell*) yamlHandle->getFeature();
                break;
            case stringCode::dice:
                dice = (Dice*) yamlHandle->getFeature();
                break;
            default:
                std::cerr << "[YamlHandleCharacter] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void* getFeature() override {
        Character* character;
        switch (getType()) {
            case stringCode::hero:
                character = new Character(name, "hero");
                break;
            case stringCode::enemy:
                character = new Character(name, "enemy");
                break;
            default:
                std::cerr << "[YamlHandleCharacter] unknown character type: " <<
                          static_cast<int>(getType()) << std::endl;
                exit(4);
        }

        character->setMaxHP(maxHP);
        if (spell) {
            character->setSpell(spell);
            character->getSpell()->setCharacter(character);
        }
        else {
            auto* spell_ = new Spell("empty", 0, 0, SpellType(SpellType::empty));
            character->setSpell(spell_);
            character->getSpell()->setCharacter(character);
        }
        character->setDice(dice);
        character->getDice()->setCharacter(character);
        character->getDice()->setName(character->getName());
        return (void*) character;
    };
};

class YamlHandleDice : public YamlHandle {
    Face* faces[6]{};

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
                faces[index] = face;
                break;
            default:
                std::cerr << "[YamlHandleDice] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void* getFeature() override {
        auto* dice = new Dice();
        for (int i = 0; i < 6; i++) {
            dice->setFace(faces[i], i);
            dice->getFace(i)->setDice(dice);
            dice->getFace(i)->setName(dice->getName());
        }
        return (void*) dice;
    };
};

class YamlHandleFace : public YamlHandle {
    int face_;
    int value = 0;
    FaceType type = {};
    FaceModifier modifiers = FaceModifier();
public:
    explicit YamlHandleFace(int face_) : YamlHandle(stringCode::face), face_(face_) {};

    void reset() override {
        value = 0;
        type = FaceType::empty;
        modifiers = FaceModifier();
    }

    void handle(YamlHandle* yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::damage:
                type = FaceType::damage;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::shield:
                type = FaceType::shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::heal:
                type = FaceType::heal;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::mana:
                type = FaceType::mana;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::shield_and_mana:
                type = FaceType::shield_and_mana;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::heal_and_shield:
                type = FaceType::heal_and_shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::damage_and_self_shield:
                type = FaceType::damage_and_self_shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::heal_and_mana:
                type = FaceType::heal_and_mana;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::dodge:
                type = FaceType::dodge;
                value = 0;
                break;
            case stringCode::empty:
                type = FaceType::empty;
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


class YamlHandleSpell : public YamlHandle {
    std::string name;
    int cost = 0;
    int value = 0;
    SpellType type = {};
public:
    explicit YamlHandleSpell() : YamlHandle(stringCode::spell) {};

    void reset() override {
        cost = 0;
        value = 0;
        type = SpellType::empty;
    }

    void handle(YamlHandle* yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::name:
                name = *(std::string*) yamlHandle->getFeature();
                break;
            case stringCode::cost:
                cost = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::damage:
                type = SpellType::damage;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::heal:
                type = SpellType::heal;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::damage_or_shield:
                type = SpellType::damage_or_shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::heal_or_shield:
                type = SpellType::heal_or_shield;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::damage_if_full_health:
                type = SpellType::damage_if_full_health;
                value = *(int*) yamlHandle->getFeature();
                break;
            case stringCode::kill_if_below_threshold:
                type = SpellType::kill_if_below_threshold;
                value = *(int*) yamlHandle->getFeature();
                break;
            default:
                std::cerr << "[YamlHandleSpell] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void* getFeature() override {
        auto* spell = new Spell(name, cost, value, type);
        return (void*) spell;
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

    static std::string trim(const std::string &str, const std::string &whitespace);

    void readFile(const std::string &string);

    YamlHandle* getData() {
        return handle;
    };

};

}

#endif //DICEGONEROGUE_YAMLREADER_H
