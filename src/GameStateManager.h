//
// Created by thijs on 07-06-22.
//

#ifndef DICEGONEROGUE_GAMESTATEMANAGER_H
#define DICEGONEROGUE_GAMESTATEMANAGER_H

#include <vector>
#include <utilities/AttackOrder.h>

#include "utilities/Window.h"
#include "gameobject/spell/Spell.h"

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
    Spell* clickedSpell = nullptr;

    std::vector<Character*> heroes;
    std::vector<Character*> enemies;

    AttackOrder* attackOrder = nullptr;

    void updateButtons();
public:
    explicit GameStateManager(Window* window) : window(window) { };

    /// getters
    [[nodiscard]] const std::vector<Character*> &getHeroes() const;

    [[nodiscard]] const std::vector<Character*> &getEnemies() const;

    [[nodiscard]] gameState getGameState() const;

    [[nodiscard]] Character* getClickedCharacter() const;

    [[nodiscard]] Spell* getClickedSpell() const;

    [[nodiscard]] bool areHeroesRolling() const;

    [[nodiscard]] bool areEnemiesRolling() const;

    [[nodiscard]] bool areHeroesAttacking() const;

    [[nodiscard]] bool areEnemiesAttacking() const;

    [[nodiscard]] int getRerolls() const;

    [[nodiscard]] int getMana() const;

    [[nodiscard]] Window* getWindow() const;

    [[nodiscard]] AttackOrder* getAttackOrder() const;

    /// setters
    void setHeroes(const std::vector<Character*> &heroes_);

    void setEnemies(const std::vector<Character*> &enemies);

    void setMana(int mana_);

    void setClickedCharacter(Character* clickedCharacter_);

    void setClickedSpell(Spell* clickedSpell_);

    void setNextGameState();

    void setAttackOrder(AttackOrder* attackOrder_);

    /// functions
    int reroll();

    void addMana(int mana_);

    std::pair<Character*, Character*> getNeighbours(Character* character);

    static std::pair<Character*, Character*> getNeighbours(Character* character, std::vector<Character*> otherCharacters);

    /// render
    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);


};

}

#endif //DICEGONEROGUE_GAMESTATEMANAGER_H
