//
// Created by thijs on 11-06-22.
//

#ifndef DICEGONEROGUE_ATTACKORDER_H
#define DICEGONEROGUE_ATTACKORDER_H

#include "gameobject/Character.h"
#include "gameobject/spell/Spell.h"
#include "gameobject/dice/Face.h"

namespace DGR {

class BattleScene;

class AttackOrder {
public:
    enum attackType : int {
        character,
        spell
    };

private:
     std::shared_ptr<BattleScene> battleScene;

    int mana{};
    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    std::vector<attackType> attackOrder{};
    std::vector<std::pair<int, int>> attackOrderIDs{};

     std::shared_ptr<Character> getStoredCharacter(int id);

     std::shared_ptr<Spell> getStoredSpell(int id);

public:
    explicit AttackOrder( std::shared_ptr<BattleScene> battleScene) : battleScene(battleScene) {};

    void addAttack( std::shared_ptr<Character> character,  std::shared_ptr<Character> otherCharacter = nullptr);

    void addAttack( std::shared_ptr<Spell> spell,  std::shared_ptr<Character> character);

    void undo();

    void setState(const std::vector<std::shared_ptr<Character>> &heroes_, const std::vector<std::shared_ptr<Character>> &enemies_, int mana_);

    void reset();
};

}

#endif //DICEGONEROGUE_ATTACKORDER_H
