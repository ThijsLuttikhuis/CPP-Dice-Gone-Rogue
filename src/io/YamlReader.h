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

class YamlHandleCharacters : public YamlHandle {
    std::vector<std::shared_ptr<Character>> characters = std::vector<std::shared_ptr<Character>>();

public:
    YamlHandleCharacters() : YamlHandle(stringCode::characters) {}

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
#if DGR_DEBUG
        std::cout << "yh.characters: adding object" << std::endl;
#endif
        switch (yamlHandle->getType()) {
            case stringCode::hero:
                characters.push_back(std::static_pointer_cast<Character>(yamlHandle->getFeature()));
                break;
            case stringCode::enemy:
                characters.push_back(std::static_pointer_cast<Character>(yamlHandle->getFeature()));
                break;
            default:
                std::cerr << "[YamlHandleCharacters] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    void reset() override {
        characters = std::vector<std::shared_ptr<Character>>();
    };

    std::shared_ptr<void> getFeature() override {
        return std::make_shared<std::vector<std::shared_ptr<Character>>>(characters);
    };
};

class YamlHandleCharacter : public YamlHandle {
    std::shared_ptr<Spell> spell = nullptr;
    std::shared_ptr<Dice> dice = nullptr;
    int maxHP{};
    int size{};
    std::string name;
public:
    explicit YamlHandleCharacter(std::string name, stringCode stringCode)
          : YamlHandle(stringCode), name(std::move(name)) {}

    void reset() override {
        spell = nullptr;
        dice = nullptr;
        maxHP = {};
    };

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::hp:
                maxHP = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::size:
                size = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::spell:
                spell = std::static_pointer_cast<Spell>(yamlHandle->getFeature());
                break;
            case stringCode::dice:
                dice = std::static_pointer_cast<Dice>(yamlHandle->getFeature());
                break;
            default:
                std::cerr << "[YamlHandleCharacter] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    std::shared_ptr<void> getFeature() override {
        std::shared_ptr<Character> character;
        switch (getType()) {
            case stringCode::hero:
                character = std::make_shared<Character>(name, "hero");
                break;
            case stringCode::enemy:
                character = std::make_shared<Character>(name, "enemy");
                break;
            default:
                std::cerr << "[YamlHandleCharacter] unknown character type: " <<
                          static_cast<int>(getType()) << std::endl;
                exit(4);
        }

        character->setMaxHP(maxHP);
        character->setSize(glm::vec2(size, size * 3 / 2));
        if (spell) {
            character->setSpell(spell);
            character->getSpell()->setCharacter(character->getSharedFromThis());
        } else {
            spell = std::make_shared<Spell>("empty", 0, 0, SpellType(SpellType::empty));
            character->setSpell(spell);
            character->getSpell()->setCharacter(character->getSharedFromThis());
        }
        character->setDice(dice);
        character->getDice()->setCharacter(character->getSharedFromThis());
        character->getDice()->setName(character->getName());
        return character;
    };
};

class YamlHandleDice : public YamlHandle {
    std::shared_ptr<Face> faces[6]{};

public:
    explicit YamlHandleDice() : YamlHandle(stringCode::dice) {};

    void reset() override {
        for (auto &face : faces) {
            face = {};
        }
    }

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
        std::shared_ptr<Face> face;
        int index;

        switch (yamlHandle->getType()) {
            case stringCode::face:
                face = std::static_pointer_cast<Face>(yamlHandle->getFeature());
                index = face->getFace_();
                faces[index] = face;
                break;
            default:
                std::cerr << "[YamlHandleDice] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    std::shared_ptr<void> getFeature() override {
        std::shared_ptr dice = std::make_shared<Dice>();
        for (int i = 0; i < 6; i++) {
            dice->setFace(faces[i], i);
            dice->getFace(i)->setDice(dice->getSharedFromThis());
            dice->getFace(i)->setName(dice->getName());
        }
        return dice;
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

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::damage:
                type = FaceType::damage;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::shield:
                type = FaceType::shield;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal:
                type = FaceType::heal;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::bonus_health:
                type = FaceType::bonus_health;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::mana:
                type = FaceType::mana;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::shield_and_mana:
                type = FaceType::shield_and_mana;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal_and_shield:
                type = FaceType::heal_and_shield;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage_and_self_shield:
                type = FaceType::damage_and_self_shield;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal_and_mana:
                type = FaceType::heal_and_mana;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage_and_mana:
                type = FaceType::damage_and_mana;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::dodge:
                type = FaceType::dodge;
                value = 0;
                break;
            case stringCode::empty:
                type = FaceType::empty;
                value = 0;
                break;
            case stringCode::mod:
                modifiers.addModifier(*std::static_pointer_cast<std::string>(yamlHandle->getFeature()).get());
                break;
            default:
                std::cerr << "[YamlHandleFace] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    std::shared_ptr<void> getFeature() override {
        return std::make_shared<Face>(face_, value, type, modifiers);
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

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::name:
                name = *std::static_pointer_cast<std::string>(yamlHandle->getFeature()).get();
                break;
            case stringCode::cost:
                cost = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage:
                type = SpellType::damage;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal:
                type = SpellType::heal;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage_or_shield:
                type = SpellType::damage_or_shield;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal_or_shield:
                type = SpellType::heal_or_shield;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage_if_full_health:
                type = SpellType::damage_if_full_health;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::kill_if_below_threshold:
                type = SpellType::kill_if_below_threshold;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::cleanse:
                type = SpellType::cleanse;
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            default:
                std::cerr << "[YamlHandleSpell] unsupported feature: " <<
                          static_cast<int>(yamlHandle->getType()) << std::endl;
                exit(4);
        }
    }

    std::shared_ptr<void> getFeature() override {
        return std::make_shared<Spell>(name, cost, value, type);
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
