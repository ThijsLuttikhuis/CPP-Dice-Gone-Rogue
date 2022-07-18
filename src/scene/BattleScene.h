//
// Created by thijs on 17-06-22.
//

#ifndef DICEGONEROGUE_BATTLESCENE_H
#define DICEGONEROGUE_BATTLESCENE_H


#include <vector>

#include "Scene.h"
#include "io/BattleLog.h"
#include "ui/Window.h"
#include "gameobject/spell/Spell.h"

namespace DGR {

class BattleScene : public std::enable_shared_from_this<BattleScene>, public Scene {
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
    bool rerunBattle = false;
    bool skipRerun = false;
    bool pauseRerun = false;
    bool allowButtonPress = true;

    std::shared_ptr<Character> clickedCharacter = nullptr;
    std::shared_ptr<Spell> clickedSpell = nullptr;

    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    std::shared_ptr<BattleLog> battleLog = nullptr;

    void alignCharacterPositions(double dt);

    bool checkVictory();

    void pressButton(const std::shared_ptr<Button> &button);

    void handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos);

    void updateButtons();

    void clickCharacter(const std::shared_ptr<Character> &character);

    void clickSpell(const std::shared_ptr<Spell> &spell);

    void enemyAttack(int index);

    int reroll();

    void setClickedCharacter(std::shared_ptr<Character> clickedCharacter_);

    void setClickedSpell(std::shared_ptr<Spell> clickedSpell_);

    void updateRerunBattle(bool reset = false);

public:
    explicit BattleScene(std::weak_ptr<GameStateManager> gameState);

    /// getters
    [[nodiscard]] std::shared_ptr<BattleScene> getSharedFromThis();

    [[nodiscard]] bool areHeroesRolling() const;

    [[nodiscard]] bool areEnemiesRolling() const;

    [[nodiscard]] bool areHeroesAttacking() const;

    [[nodiscard]] bool areEnemiesAttacking() const;

    [[nodiscard]] battleGameState getState() const;

    [[nodiscard]]  std::shared_ptr<Character> getClickedCharacter() const;

    [[nodiscard]]  std::shared_ptr<Spell> getClickedSpell() const;

    [[nodiscard]]  std::shared_ptr<BattleLog> getBattleLog() const;

    [[nodiscard]] int getMana() const;

    [[nodiscard]] int getRerolls() const;

    std::pair<std::shared_ptr<Character>, std::shared_ptr<Character>> getNeighbours(
          const std::shared_ptr<Character> &character) const;

    std::vector<std::shared_ptr<Character>> getAliveCharacters(bool aliveHeroes);

    /// setters
    void setNextGameState(bool doSaveTurn = true);

    void addMana(int mana_);

    void setMana(int mana_);

    void setEnemies(const std::vector<std::shared_ptr<Character>> &enemies_);

    void setHeroes(const std::vector<std::shared_ptr<Character>> &heroes_);

    void setCharactersFromBattleLog();

    void setBattleLog(const std::shared_ptr<BattleLog> &battleLog_);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void rerunBattleFromStart();

    void initialize() override;

    void onPopFromStack() override;

    void reset() override;

    void update(double dt) override;

    std::string message(const std::string &data) override;

    /// render
    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) override;

};

}


#endif //DICEGONEROGUE_BATTLESCENE_H
