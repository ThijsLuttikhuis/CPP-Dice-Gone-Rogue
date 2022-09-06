//
// Created by thijs on 03-09-22.
//

#ifndef DICEGONEROGUE_YAMLREADERCHARACTERS_H
#define DICEGONEROGUE_YAMLREADERCHARACTERS_H

#include <gameobject/dice/face/FaceDamage.h>
#include <gameobject/dice/face/FaceEmpty.h>
#include <gameobject/dice/face/FaceShield.h>
#include "YamlReader.h"

namespace DGR {

class YamlHandleCharacters : public YamlHandle {
    std::vector<std::shared_ptr<Character>> characters = std::vector<std::shared_ptr<Character >>();

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
        characters = std::vector<std::shared_ptr<Character >>();
    };

    std::shared_ptr<void> getFeature() override {
        return std::make_shared<std::vector<std::shared_ptr<Character >>>(characters);
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
    const int face_;
    int faceBonus = 1;
    int value = 0;
    std::shared_ptr<Face> face = {};
    FaceModifier modifiers = FaceModifier();
public:
    explicit YamlHandleFace(int face_) : YamlHandle(stringCode::face), face_(face_) {};

    void reset() override {
        value = {};
        faceBonus = {};
        modifiers = FaceModifier();
        face = {};
    }

    void handle(std::shared_ptr<YamlHandle> yamlHandle) override {
        switch (yamlHandle->getType()) {
            case stringCode::face_bonus:
                faceBonus = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage:
                face = std::make_shared<FaceDamage>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::shield:
                face = std::make_shared<FaceShield>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::bonus_health:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::mana:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::shield_and_mana:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal_and_shield:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage_and_self_shield:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::heal_and_mana:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::damage_and_mana:
                face = std::make_shared<FaceEmpty>(face_);
                value = *std::static_pointer_cast<int>(yamlHandle->getFeature()).get();
                break;
            case stringCode::dodge:
                face = std::make_shared<FaceEmpty>(face_);
                value = 0;
                break;
            case stringCode::empty:
                face = std::make_shared<FaceEmpty>(face_);
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
        if (!face) {
            std::cerr << "[YamlHandleFace] face not set!" << std::endl;
            exit(4);
        }
        face->setValue(value);
        face->setBonusValuePerLevel(faceBonus);
        face->setModifiers(modifiers);

        return face;
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

}

#endif //DICEGONEROGUE_YAMLREADERCHARACTERS_H
