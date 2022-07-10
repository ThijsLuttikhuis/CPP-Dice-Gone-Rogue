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
        bool heroesAttacked;

        static Turn getTurnFromString(std::string turnString);

        [[nodiscard]] std::string toString() const;
    };

private:
    std::weak_ptr<BattleScene> battleScene;

    int mana{};
    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    static int heroesTurnCounter;
    static int enemiesTurnCounter;
    Turn thisTurn;
    std::vector<Turn> turns;

    std::shared_ptr<Character> getStoredCharacter(int id);

    std::shared_ptr<Spell> getStoredSpell(int id);

    [[nodiscard]] std::string idsCharacternamesToString() const;

    [[nodiscard]] std::string idsFaceidsToString() const;

    static std::string loadBattleHeader(const std::shared_ptr<GameStateManager> &gameState,
                                        std::string battleString, std::shared_ptr<BattleLog> &battleLog);

public:
    BattleLog() = default;

    explicit BattleLog(std::weak_ptr<BattleScene> battleScene);

    void addAttack(Character* character, Character* otherCharacter = nullptr);

    void addAttack(Spell* spell, Character* character);

    void undo();

    void saveTurn(bool heroesAttacked);

    static std::shared_ptr<BattleLog> loadBattle(const std::shared_ptr<GameStateManager> &gameState,
                                                 const std::string &fileName);

    void setState(const std::vector<std::shared_ptr<Character>> &heroes_,
                  const std::vector<std::shared_ptr<Character>> &enemies_, int mana_);

    [[nodiscard]] std::tuple<std::vector<std::shared_ptr<Character>>*,
          std::vector<std::shared_ptr<Character>>*,
          int> getState();

    void reset();

};

}

#endif //DICEGONEROGUE_BATTLELOG_H
