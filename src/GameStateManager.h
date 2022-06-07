//
// Created by thijs on 07-06-22.
//

#ifndef DICEGONEROGUE_GAMESTATEMANAGER_H
#define DICEGONEROGUE_GAMESTATEMANAGER_H

#include <vector>
#include "gameobject/Hero.h"
#include "gameobject/Enemy.h"

namespace DGR {

class GameStateManager {
public:
    enum gameState {
        rolling_heroes,
        rolling_enemies,
        attack_block_heroes,
        attack_block_enemies
    };
private:
    bool heroesTurn = true;
    gameState state = rolling_heroes;
    int rerolls = 3;
    int mana = 0;

    Character* clickedCharacter = nullptr;

    std::vector<Hero*> heroes;

    std::vector<Enemy*> enemies;
public:
    [[nodiscard]] const std::vector<Hero*> &getHeroes() const;

    [[nodiscard]] const std::vector<Enemy*> &getEnemies() const;

    [[nodiscard]] gameState getGameState() const;

    [[nodiscard]] bool isHeroesTurn() const;

    [[nodiscard]] Character* getClickedCharacter() const;

    void setHeroes(const std::vector<Hero*> &heroes_);

    void setEnemies(const std::vector<Enemy*> &enemies);

    void setClickedCharacter(Character* clickedCharacter_);

    int reroll();

    void setGameState(gameState state);

    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    void addMana(int mana_);
};

}

#endif //DICEGONEROGUE_GAMESTATEMANAGER_H
