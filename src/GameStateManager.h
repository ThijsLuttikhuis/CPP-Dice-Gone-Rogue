//
// Created by thijs on 07-06-22.
//

#ifndef DICEGONEROGUE_GAMESTATEMANAGER_H
#define DICEGONEROGUE_GAMESTATEMANAGER_H

#include <vector>
#include <utilities/Window.h>
#include "gameobject/Hero.h"
#include "gameobject/Enemy.h"

namespace DGR {

class GameStateManager {
public:
    enum gameState {
        rolling_heroes,
        attack_block_heroes,
        rolling_enemies,
        attack_block_enemies
    };
private:
    gameState state = rolling_heroes;
    int rerollsMax = 3;
    int rerolls = rerollsMax;
    int mana = 0;

    Window* window;

    Character* clickedCharacter = nullptr;

    std::vector<Hero*> heroes;

    std::vector<Enemy*> enemies;
public:
    explicit GameStateManager(Window* window) : window(window) {};

    [[nodiscard]] const std::vector<Hero*> &getHeroes() const;

    [[nodiscard]] const std::vector<Enemy*> &getEnemies() const;

    [[nodiscard]] gameState getGameState() const;

    [[nodiscard]] Character* getClickedCharacter() const;

    [[nodiscard]] bool areHeroesRolling() const;

    [[nodiscard]] bool areEnemiesRolling() const;

    [[nodiscard]] bool areHeroesAttacking() const;

    [[nodiscard]] bool areEnemiesAttacking() const;

    [[nodiscard]] Window* getWindow() const;

    void setHeroes(const std::vector<Hero*> &heroes_);

    void setEnemies(const std::vector<Enemy*> &enemies);

    void setClickedCharacter(Character* clickedCharacter_);

    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    int reroll(bool rollHeroes = true);

    void addMana(int mana_);

    void setNextGameState();

    void updateButtons();

    std::pair<Character*, Character*> getNeighbours(Character* character);
};

}

#endif //DICEGONEROGUE_GAMESTATEMANAGER_H
