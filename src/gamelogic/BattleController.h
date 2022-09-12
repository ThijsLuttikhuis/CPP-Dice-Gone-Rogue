//
// Created by thijs on 09-09-22.
//

#ifndef DICEGONEROGUE_BATTLECONTROLLER_H
#define DICEGONEROGUE_BATTLECONTROLLER_H

#include <gamelogic/BattleLog.h>
#include "gameobject/Character.h"

namespace DGR {

class BattleController : public std::enable_shared_from_this<BattleController> {
public:
    enum battleGameState {
        rolling_heroes,
        attack_block_heroes,
        rolling_enemies,
        attack_block_enemies
    };

    enum battleType {
        play,
        replay,
        load_play
    };

private:
    std::unique_ptr<BattleLog> battleLog = nullptr;

    std::weak_ptr<GameStateManager> gameState;

    int animationCounter = 0;
    bool rerunBattle = false;
    bool skipRerun = false;
    bool pauseRerun = false;

    int level = 0;
    const int rerollsMax = 3;
    int rerolls = rerollsMax;
    int mana = 0;
    battleGameState state = rolling_enemies;
    battleType battleType = play;

    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    std::shared_ptr<Character> clickedCharacter = nullptr;
    std::shared_ptr<Spell> clickedSpell = nullptr;


public:
    explicit BattleController(std::weak_ptr<GameStateManager> gameState);

    std::shared_ptr<BattleController> getSharedFromThis();

    std::weak_ptr<GameStateManager> getGameStateManager();

    bool areHeroesRolling() const;

    bool areEnemiesRolling() const;

    bool areHeroesAttacking() const;

    bool areEnemiesAttacking() const;

    std::vector<std::shared_ptr<Character>> &getHeroes();

    std::vector<std::shared_ptr<Character>> &getEnemies();

    int getRerolls() const;

    int getMana() const;

    std::vector<std::shared_ptr<Character>> getAliveCharacters(bool aliveHeroes);

    std::pair<std::shared_ptr<Character>, std::shared_ptr<Character>> getNeighbours(
          const std::shared_ptr<Character> &character) const;

    battleGameState getState() const;

    std::shared_ptr<Character> getClickedCharacter() const;

    std::shared_ptr<Spell> getClickedSpell() const;

    const std::unique_ptr<BattleLog> &getBattleLog() const;

    void setEnemiesFromLevel(int selectedLevel);

    void setEnemies(const std::vector<std::shared_ptr<Character>> &enemies_);

    void setHeroes(const std::vector<std::shared_ptr<Character>> &heroes_);

    void setNextGameState(bool doSaveTurn = true);

    void setMana(int mana_);

    void addMana(int mana_);

    void setBattleLog(std::unique_ptr<BattleLog> battleLog_);

    void setClickedCharacter(std::shared_ptr<Character> clickedCharacter_);

    void setClickedSpell(std::shared_ptr<Spell> clickedSpell_);

    void update(double dt);

    bool checkVictory();

    void enemyAttack(int index);

    int reroll();

    void initialize();

    void reset();

    void setCharactersFromBattleLog();

    void rerunBattleFromStart();

    void updateRerunBattle(bool reset = false);

    void pressLeftButton(const std::unique_ptr<Button> &button);

    void pressRightButton(const std::unique_ptr<Button> &button);

    void clickCharacter(const std::shared_ptr<Character> &character);

    void clickSpell(const std::shared_ptr<Spell> &spell);
};

}

#endif //DICEGONEROGUE_BATTLECONTROLLER_H
