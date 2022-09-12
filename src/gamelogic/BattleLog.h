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
#include "gameobject/dice/face/Face.h"
#include "utilities/Utilities.h"

namespace DGR {

class BattleController;

class BattleLog {
public:
    enum attackType : int {
        character,
        spell
    };

    struct Turn {
        std::vector<std::shared_ptr<Character>> heroesAtStart;
        std::vector<std::shared_ptr<Character>> enemiesAtStart;

        std::map<int, int> idsToCurrentFace = {};
        std::vector<attackType> attackOrder = {};
        std::vector<std::pair<int, int>> attackOrderIDs = {};
        bool heroesTurn;

        static std::string getTurnFromString(const std::shared_ptr<GameStateManager> &gameState,
                                             std::string battleString,
                                             std::unique_ptr<BattleLog> &battleLog);

        [[nodiscard]] std::string toString() const;
    };

private:
    Turn thisTurn;
    std::vector<Turn> turns;

    std::weak_ptr<BattleController> battleController;

    int mana{};
    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    int heroesTurnCounter = 0;
    int enemiesTurnCounter = 0;

    std::shared_ptr<Character> getStoredCharacter(int id);

    std::shared_ptr<Spell> getStoredSpell(int id);

    [[nodiscard]] std::string idsCharacternamesToString() const;

    static std::string loadBattleHeader(const std::shared_ptr<GameStateManager> &gameState,
                                        std::string battleString, std::unique_ptr<BattleLog> &battleLog);

public:
    BattleLog() = default;

    explicit BattleLog(std::weak_ptr<BattleController> battleController);

    /// getters
    static std::unique_ptr<BattleLog> loadBattle(const std::shared_ptr<GameStateManager> &gameState,
                                                 const std::string &fileName);

    [[nodiscard]] std::tuple<std::vector<std::shared_ptr<Character>>*,
          std::vector<std::shared_ptr<Character>>*, int> getState();

    /// setters
    void setState(const std::vector<std::shared_ptr<Character>> &heroes_,
                  const std::vector<std::shared_ptr<Character>> &enemies_, int mana_);

    void setBattleController(std::weak_ptr<BattleController> battleController_);

    void addAttack(Character* character, Character* otherCharacter = nullptr);

    void addAttack(Spell* spell, Character* character);

    /// functions
    void reset();

    void undo();

    void saveTurn(bool heroesAttacked);

    bool doRerunBattleAttack(int &currentTurn, int &currentAttack);

    void clearAutoSave();

};

}

#endif //DICEGONEROGUE_BATTLELOG_H
