//
// Created by thijs on 17-06-22.
//

#ifndef DICEGONEROGUE_BATTLESCENE_H
#define DICEGONEROGUE_BATTLESCENE_H


#include <vector>

#include "Scene.h"
#include "utilities/AttackOrder.h"
#include "ui/Window.h"
#include "gameobject/spell/Spell.h"

namespace DGR {

class BattleScene : public Scene {
public:
    enum battleGameState {
        rolling_heroes,
        attack_block_heroes,
        rolling_enemies,
        attack_block_enemies
    };
private:
    const int rerollsMax = 3;
    int rerolls = rerollsMax;
    int mana = 0;
    battleGameState state = rolling_enemies;

    int animationCounter = 0;

    std::shared_ptr<Character> clickedCharacter = nullptr;
    std::shared_ptr<Spell> clickedSpell = nullptr;

    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    std::shared_ptr<AttackOrder> attackOrder = nullptr;

    void alignCharacterPositions(double dt);

    void checkVictory();

    void pressButton(std::shared_ptr<Button> button);

    void handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos);

    void updateButtons();

    void clickCharacter(std::shared_ptr<Character> character);

    void clickSpell(std::shared_ptr<Spell> spell);

    void enemyAttack(int index);

    int reroll();

    void setClickedCharacter(std::shared_ptr<Character> clickedCharacter_);

    void setClickedSpell(std::shared_ptr<Spell> clickedSpell_);

public:
    explicit BattleScene(std::shared_ptr<GameStateManager> gameState);

    /// getters
    [[nodiscard]] bool areHeroesRolling() const;

    [[nodiscard]] bool areEnemiesRolling() const;

    [[nodiscard]] bool areHeroesAttacking() const;

    [[nodiscard]] bool areEnemiesAttacking() const;

    [[nodiscard]] battleGameState getState() const;

    [[nodiscard]]  std::shared_ptr<Character> getClickedCharacter() const;

    [[nodiscard]]  std::shared_ptr<Spell> getClickedSpell() const;

    [[nodiscard]]  std::shared_ptr<AttackOrder> getAttackOrder() const;

    [[nodiscard]] int getMana() const;

    [[nodiscard]] int getRerolls() const;

    /// setters
    void setNextGameState();

    void addMana(int mana_);

    void setMana(int mana_);

    void setEnemies(const std::vector<std::shared_ptr<Character>> &enemies_);

    void setHeroes(const std::vector<std::shared_ptr<Character>> &heroes_);

    void setAttackOrder(std::shared_ptr<AttackOrder> attackOrder_);

    /// functions
    std::pair<std::shared_ptr<Character>, std::shared_ptr<Character>> getNeighbours(std::shared_ptr<Character> character);

    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void reset() override;

    void update(double dt) override;

    /// render
    void render(std::shared_ptr<SpriteRenderer> spriteRenderer, std::shared_ptr<TextRenderer> textRenderer) override;

};

}


#endif //DICEGONEROGUE_BATTLESCENE_H
