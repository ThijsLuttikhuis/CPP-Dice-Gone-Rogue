//
// Created by thijs on 11-06-22.
//

#ifndef DICEGONEROGUE_ATTACKORDER_H
#define DICEGONEROGUE_ATTACKORDER_H

#include <gameobject/Enemy.h>
#include <gameobject/Hero.h>
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
    std::vector<Hero*> heroes;
    std::vector<Enemy*> enemies;

    std::vector<attackType> attackOrder{};
    std::vector<ChCh> characterAttacks{};
    std::vector<SpCh> characterSpells{};

public:
    explicit AttackOrder(GameStateManager* gameState) : gameState(gameState) {};

    void addAttack(Character* character, Character* otherCharacter = nullptr);

    void addAttack(Spell* spell, Character* character);

    void undo();

    void setCharacters(const std::vector<Hero*> &heroes_, const std::vector<Enemy*> &enemies_);
};

}

#endif //DICEGONEROGUE_ATTACKORDER_H
