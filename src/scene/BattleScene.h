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

    Character* clickedCharacter = nullptr;
    Spell* clickedSpell = nullptr;

    std::vector<Character*> heroes;
    std::vector<Character*> enemies;

    AttackOrder* attackOrder = nullptr;

    void alignCharacterPositions(double dt);

    void checkVictory();

    void pressButton(Button* button);

    void handleMousePosition(Character* character, double xPos, double yPos);

    void updateButtons();

    void clickCharacter(Character* character);

    void clickSpell(Spell* spell);

    void enemyAttack(int index);

    int reroll();

public:
    explicit BattleScene(GameStateManager* gameState);

    /// getters
    [[nodiscard]] bool areHeroesRolling() const;

    [[nodiscard]] bool areEnemiesRolling() const;

    [[nodiscard]] bool areHeroesAttacking() const;

    [[nodiscard]] bool areEnemiesAttacking() const;

    [[nodiscard]] battleGameState getState() const;

    [[nodiscard]] Character* getClickedCharacter() const;

    [[nodiscard]] Spell* getClickedSpell() const;

    [[nodiscard]] AttackOrder* getAttackOrder() const;

    [[nodiscard]] int getMana() const;

    [[nodiscard]] int getRerolls() const;

    /// setters
    void setNextGameState();

    void addMana(int mana_);

    void setMana(int mana_);

    void setEnemies(const std::vector<Character*> &enemies_);

    void setHeroes(const std::vector<Character*> &heroes_);

    void setClickedCharacter(Character* clickedCharacter_);

    void setAttackOrder(AttackOrder* attackOrder_);

    void setClickedSpell(Spell* clickedSpell_);

    /// functions
    std::pair<Character*, Character*> getNeighbours(Character* character);

    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void reset() override;

    void update(double dt) override;

    /// render
    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;

};

}


#endif //DICEGONEROGUE_BATTLESCENE_H
