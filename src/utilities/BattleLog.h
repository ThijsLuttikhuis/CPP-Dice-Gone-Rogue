//
// Created by thijs on 11-06-22.
//

#ifndef DICEGONEROGUE_BATTLELOG_H
#define DICEGONEROGUE_BATTLELOG_H

#include <utility>
#include <iostream>
#include <string>

#include "gameobject/Character.h"
#include "gameobject/spell/Spell.h"
#include "gameobject/dice/Face.h"
#include "Utilities.h"

namespace DGR {

class BattleScene;

class BattleLog {
public:
    enum attackType : int {
        character,
        spell
    };

    struct Turn {
        std::vector<attackType> attackOrder = {};
        std::vector<std::pair<int, int>> attackOrderIDs = {};

        static Turn getTurnFromString(std::string turnString) {
            Turn turn;
            std::pair<int, int> ids;
            int idNotSet = -12345;
            turnString = Utilities::trim(turnString);
            std::string word;
            size_t i = 0;
            auto turnStrLength = turnString.size();

            while (i < turnStrLength) {
                size_t posComma = turnString.find(',', i);
                word = Utilities::trim(turnString.substr(i, posComma - i));
                i = posComma + 1;

                if (word == "character") {
                    turn.attackOrder.push_back(attackType::character);
                }
                else if (word == "spell") {
                    turn.attackOrder.push_back(attackType::spell);
                }
                else if (word.substr(0, 4) == "dst:") {
                    char** ptr = nullptr;
                    ids.first = (int) strtol(word.c_str() + 4, ptr, 10);
                }
                else if (word.substr(0, 4) == "src:") {
                    char** ptr = nullptr;
                    ids.second = (int) strtol(word.c_str() + 4, ptr, 10);
                }

                if (ids.first != idNotSet && ids.second != idNotSet) {
                    turn.attackOrderIDs.push_back(ids);
                    ids.first = idNotSet;
                    ids.second = idNotSet;
                }

            }

            return turn;
        }

        std::string toString() {
            std::string turnString;
            for (int i = 0; i < (int) attackOrder.size(); i++) {
                turnString += attackOrder[i] == attackType::character ? "character, dst: " : "spell, dst: ";
                turnString += std::to_string(attackOrderIDs[i].first) + ", src: " +
                              std::to_string(attackOrderIDs[i].second) + ";\n";
            }
            std::cout << turnString << std::endl;
            return turnString;
        }
    };

private:
    std::weak_ptr<BattleScene> battleScene;

    int mana{};
    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    Turn thisTurn;
    std::vector<Turn> turns;

    std::shared_ptr<Character> getStoredCharacter(int id);

    std::shared_ptr<Spell> getStoredSpell(int id);

    void printCharacterIDs();
public:
    explicit BattleLog(std::weak_ptr<BattleScene>
                       battleScene);

    void addAttack(Character* character, Character* otherCharacter = nullptr);

    void addAttack(Spell* spell, Character* character);

    void undo();

    void saveTurn();

    void setState(const std::vector<std::shared_ptr<Character>> &heroes_,
                  const std::vector<std::shared_ptr<Character>> &enemies_, int mana_);

    void reset();
};

}

#endif //DICEGONEROGUE_BATTLELOG_H
