//
// Created by thijs on 11-06-22.
//

#ifndef DICEGONEROGUE_ATTACKORDER_H
#define DICEGONEROGUE_ATTACKORDER_H

#include "gameobject/Character.h"
#include "gameobject/spell/Spell.h"
#include "gameobject/dice/Face.h"

namespace DGR {

class AttackOrder {
public:
    enum attackType : int {
        character,
        spell
    };

private:

    struct ChCh {
        Character* character;
        Character* otherCharacter;
    };
    struct SpCh {
        Spell* spell;
        Character* character;
    };

    GameStateManager* gameState;

    int mana{};
    std::vector<Character*> heroes;
    std::vector<Character*> enemies;

    std::vector<attackType> attackOrder{};
    std::vector<std::pair<int, int>> attackOrderIDs{};

    Character* getStoredCharacter(int id);

    Spell* getStoredSpell(int id);

public:
    explicit AttackOrder(GameStateManager* gameState) : gameState(gameState) {};

    void addAttack(Character* character, Character* otherCharacter = nullptr);

    void addAttack(Spell* spell, Character* character);

    void undo();

    void setState(const std::vector<Character*> &heroes_, const std::vector<Character*> &enemies_, int mana_);

};

}

#endif //DICEGONEROGUE_ATTACKORDER_H
