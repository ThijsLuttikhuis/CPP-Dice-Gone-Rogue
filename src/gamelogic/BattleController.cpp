//
// Created by thijs on 09-09-22.
//

#include <utilities/Random.h>
#include "BattleController.h"
#include "GameProgress.h"
#include "GameStateManager.h"
#include "scene/Scene.h"

namespace DGR {

BattleController::BattleController(std::weak_ptr<GameStateManager> gameState)
      : gameState(std::move(gameState)) {
}

std::shared_ptr<BattleController> BattleController::getSharedFromThis() {
    return shared_from_this();
}

std::weak_ptr<GameStateManager> BattleController::getGameStateManager() {
    return gameState;
}

BattleController::battleGameState BattleController::getState() const {
    return state;
}

std::shared_ptr<Character> BattleController::getClickedCharacter() const {
    return clickedCharacter;
}

std::shared_ptr<Spell> BattleController::getClickedSpell() const {
    return clickedSpell;
}

const std::unique_ptr<BattleLog> &BattleController::getBattleLog() const {
    return battleLog;
}

int BattleController::getRerolls() const {
    return rerolls;
}

int BattleController::getMana() const {
    return mana;
}

bool BattleController::areHeroesRolling() const {
    return state == rolling_heroes;
}

bool BattleController::areEnemiesRolling() const {
    return state == rolling_enemies;
}

bool BattleController::areHeroesAttacking() const {
    return state == attack_block_heroes;
}

bool BattleController::areEnemiesAttacking() const {
    return state == attack_block_enemies;
}

std::vector<std::shared_ptr<Character>> BattleController::getAliveCharacters(bool aliveHeroes) {
    std::vector<std::shared_ptr<Character>> aliveCharacters;
    for (auto &character : (aliveHeroes ? heroes : enemies)) {
        if (!character->isDead()) {
            aliveCharacters.push_back(character);
        }
    }
    return aliveCharacters;
}

std::pair<std::shared_ptr<Character>, std::shared_ptr<Character>> BattleController::getNeighbours(
      const std::shared_ptr<Character> &character) const {

    std::pair<std::shared_ptr<Character>, std::shared_ptr<Character>> neighbours(nullptr, nullptr);
    auto &characters = character->getCharacterType() == "hero" ? heroes :
                       character->getCharacterType() == "enemy" ? enemies : std::vector<std::shared_ptr<Character>>();

    int nCharacters = characters.size();
    for (int i = 0; i < nCharacters; i++) {
        if (character->getUniqueID() == characters[i]->getUniqueID()) {
            int j = i - 1;
            while (j >= 0) {
                if (!characters[j]->isDead()) {
                    neighbours.first = characters[j];
                    break;
                }
                j--;
            }
            j = i + 1;
            while (j < nCharacters) {
                if (!characters[j]->isDead()) {
                    neighbours.second = characters[j];
                    break;
                }
                j++;
            }
            return neighbours;
        }
    }
    return neighbours;
}


void BattleController::setBattleLog(std::unique_ptr<BattleLog> battleLog_) {
    battleLog = std::move(battleLog_);
    battleLog->setBattleController(getSharedFromThis());
}

void BattleController::setClickedCharacter(std::shared_ptr<Character> clickedCharacter_) {
    clickedCharacter = std::move(clickedCharacter_);
}

void BattleController::setClickedSpell(std::shared_ptr<Spell> clickedSpell_) {
    clickedSpell = std::move(clickedSpell_);
}

void BattleController::setMana(int mana_) {
    mana = mana_;
}

void BattleController::addMana(int mana_) {
    mana += mana_;
}

void BattleController::setHeroes(const std::vector<std::shared_ptr<Character>> &heroes_) {
    heroes = heroes_;
}

void BattleController::setEnemies(const std::vector<std::shared_ptr<Character>> &enemies_) {
    enemies = enemies_;
}

void BattleController::setEnemiesFromLevel(int selectedLevel) {
    level = selectedLevel;
    enemies = {};

    int strengthBudget = GameProgress::levelToEnemyStrength(selectedLevel);
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
    auto allEnemies = gameStatePtr->getAllEnemies();

    while (strengthBudget > 0) {
        int rng = Random::randInt(0, (int) allEnemies.size() - 1);
        auto enemy = allEnemies[rng];
        int enemyStrength = GameProgress::enemyNameToStrength(enemy->getName());
        if (enemyStrength <= strengthBudget) {
            enemies.push_back(enemy->makeCopy(false));
            strengthBudget -= enemyStrength;
        }
    }
}

void BattleController::setCharactersFromBattleLog() {
    auto battleLogState = battleLog->getState();
    heroes = *std::get<0>(battleLogState);
    enemies = *std::get<1>(battleLogState);
    mana = std::get<2>(battleLogState);

}

void BattleController::rerunBattleFromStart() {
    rerunBattle = true;
    setCharactersFromBattleLog();
}

void BattleController::updateRerunBattle(bool reset) {
    static int turnIndex = 0;
    static int attackIndex = 0;
    static bool initState = false;

    if (reset) {
        turnIndex = 0;
        attackIndex = 0;
        initState = false;
        rerunBattle = false;
        skipRerun = false;
        pauseRerun = false;
        return;
    }

    if (!initState) {
        state = battleGameState::attack_block_enemies;
        initState = true;
    }

    int slowDown = 30 / DGR_ANIMATION_SPEED;
    animationCounter++;
    if (pauseRerun || (!skipRerun && animationCounter % slowDown != 0)) {
        return;
    }
    animationCounter = 0;

    bool done = battleLog->doRerunBattleAttack(turnIndex, attackIndex);
    setCharactersFromBattleLog();
    if (done) {
        turnIndex = 0;
        attackIndex = 0;
        initState = false;
        rerunBattle = false;
        skipRerun = false;
        pauseRerun = false;
    }
}

bool BattleController::checkVictory() {
    bool allEnemiesDead = true;
    for (auto &enemy : enemies) {
        if (!enemy->isDead()) {
            allEnemiesDead = false;
            break;
        }
    }
    if (allEnemiesDead) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

        gameStatePtr->getGameProgress()->completeLevel(level, gameStatePtr->getInventory());
        gameStatePtr->pushSceneToStack("BattleVictoryScene", true);
        updateRerunBattle(true);

        return true;
    }

    bool allHeroesDead = true;
    for (auto &hero : heroes) {
        if (!hero->isDead()) {
            allHeroesDead = false;
            break;
        }
    }
    if (allHeroesDead) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        gameStatePtr->pushSceneToStack("BattleDefeatScene", true);
        updateRerunBattle(true);
        return true;
    }
    return false;
}


int BattleController::reroll() {
    if (rerolls > 0) {

        // do not reroll if all hero dice are locked
        if (areHeroesRolling()) {
            bool allDiceLocked = true;
            for (auto &hero : getAliveCharacters(true)) {
                if (!hero->getDice()->isLocked()) {
                    allDiceLocked = false;
                    break;
                }
            }

            if (allDiceLocked) {
                std::cerr << "GameStateManager::reroll: warning, all dice locked!" << std::endl;

                return rerolls;
            }
        }

        rerolls--;

        if (areHeroesRolling()) {
            for (auto &hero : heroes) {
                hero->roll();
            }
        } else if (areEnemiesRolling()) {
            for (auto &enemy : enemies) {
                enemy->roll();
            }
        } else {
            std::cerr << "GameStateManager::reroll: error, not in a rolling phase!" << std::endl;
        }
    }
#ifdef DGR_DEBUG
    else {
        std::cerr << "GameStateManager::reroll: warning, no rerolls remaining!" << std::endl;
    }
#endif
    return rerolls;
}


void BattleController::setNextGameState(bool doSaveTurn) {
    switch (state) {
        case rolling_heroes:
            for (auto &hero : heroes) {
                hero->setDiceLock(false);
            }
            rerolls = rerollsMax;
            battleLog->setState(heroes, enemies, mana);

            state = attack_block_heroes;
            break;
        case attack_block_heroes:
            if (doSaveTurn) {
                battleLog->saveTurn(true);
            }

            for (auto &hero : heroes) {
                hero->setUsedDice(false);
                hero->applyDamageStep();
            }

            state = rolling_enemies;
            reroll();
            break;
        case rolling_enemies:
            for (auto &enemy : enemies) {
                enemy->setDiceLock(false);
            }
            rerolls = rerollsMax;
            battleLog->setState(heroes, enemies, mana);

            state = attack_block_enemies;
            break;
        case attack_block_enemies:
            if (doSaveTurn) {
                battleLog->saveTurn(false);
            }
            for (auto &enemy : enemies) {
                enemy->setUsedDice(false);
                enemy->applyDamageStep();
            }

            state = rolling_heroes;
            reroll();
            break;
    }
}

void BattleController::reset() {
    rerolls = rerollsMax;
    mana = 0;
    state = rolling_enemies;
    animationCounter = 0;
    rerunBattle = false;

    clickedCharacter = nullptr;
    clickedSpell = nullptr;

    battleLog->reset();

    heroes = {};
    enemies = {};
}

void BattleController::initialize() {
    battleLog = std::make_unique<BattleLog>(getSharedFromThis());
}

void BattleController::update(double dt) {
    (void) dt;

    if (checkVictory()) {
        return;
    }

    if (rerunBattle) {
        updateRerunBattle();
        return;
    }

    //TODO: add animations etc
    int slowDown = 30 / DGR_ANIMATION_SPEED;
    if (areEnemiesRolling()) {
        animationCounter++;
        if (animationCounter % slowDown == 0) {
            reroll();
            for (auto &enemy : enemies) {
                auto currentFace = enemy->getDice()->getCurrentFace()->getFace_();
                //TODO: make better A.I :)
                if (currentFace == 0 || currentFace == 2) {
                    enemy->setDiceLock(true);
                }
            }
        }
        if (getRerolls() == 0) {
            setNextGameState();
            animationCounter = 0;
        }
    } else if (areEnemiesAttacking()) {
        if (animationCounter < (int) enemies.size() * slowDown) {
            if ((animationCounter++ % slowDown) == 0) {
                int i = animationCounter / slowDown;
                enemyAttack(i);
            }
        } else {
            animationCounter = 0;
            setNextGameState();
        }
    }
}

void BattleController::enemyAttack(int index) {
    auto enemy = enemies[index];
    bool success;

    if (enemy->isDead()) {
        return;
    }

    // check if you can interact with yourself
    success = enemy->interact(std::shared_ptr<Character>(nullptr), getSharedFromThis());

    // check if you can interact with an ally (=enemy from their perspective)
    if (!success) {
        auto aliveEnemies = getAliveCharacters(false);
        int mostIncomingDamage = 0;
        for (auto &otherEnemy : aliveEnemies) {
            mostIncomingDamage = std::max(mostIncomingDamage, otherEnemy->getIncomingDamage());
        }
        if (mostIncomingDamage > 0) {
            for (auto &otherEnemy : aliveEnemies) {
                if (otherEnemy->getIncomingDamage() == mostIncomingDamage) {
                    success = otherEnemy->interact(enemy, getSharedFromThis());
                }
            }
        } else {
            int rng = Random::randInt(0, (int) aliveEnemies.size() - 1);
            success = aliveEnemies[rng]->interact(enemy, getSharedFromThis());
        }
    }

    // finally, check if you can interact with an enemy (=hero from their perspective)
    if (!success) {
        auto aliveHeroes = getAliveCharacters(true);
        int rng = Random::randInt(0, (int) aliveHeroes.size() - 1);
        success = aliveHeroes[rng]->interact(enemy, getSharedFromThis());
    }

    if (!success) {
        std::cerr << "GameController::enemiesAttack(): ERROR in attack logic!" << std::endl;
    }
}

std::vector<std::shared_ptr<Character>> &BattleController::getHeroes() {
    return heroes;
}

std::vector<std::shared_ptr<Character>> &BattleController::getEnemies() {
    return enemies;
}

void BattleController::pressLeftButton(const std::unique_ptr<Button> &button) {
    if (rerunBattle) {
        pauseRerun = !pauseRerun;
        button->setText(pauseRerun ? "Resume" : "Pause");
        return;
    }

    if (areHeroesRolling()) {
        int rerollsLeft = reroll();
        button->setText(std::to_string(rerollsLeft) + " rerolls left");
        if (rerollsLeft == 0) {
            setNextGameState();
        }
    } else if (areHeroesAttacking()) {
        getBattleLog()->undo();
    }
}

void BattleController::pressRightButton(const std::unique_ptr<Button> &button) {
    if (rerunBattle) {
        skipRerun = !skipRerun;
        button->setText(skipRerun ? "Cancel skip" : "Skip");
        return;
    }


    if (areHeroesRolling()) {
        setNextGameState();
    } else if (areHeroesAttacking()) {
        /// check if all dice are used, otherwise ask are you sure
        bool allDiceUsed = true;
        for (auto &hero : getAliveCharacters(true)) {
            if (!hero->getDice()->isUsed()) {
                allDiceUsed = false;
                break;
            }
        }
        if (!allDiceUsed) {
            auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
            gameStatePtr->getScene("AreYouSureScene")->message(
                  "BattleScene: nextGameState");

            gameStatePtr->pushSceneToStack("AreYouSureScene");
        } else {
            setNextGameState();
        }
    }
}

void BattleController::clickCharacter(const std::shared_ptr<Character> &character) {
    bool success;

    switch (state) {
        case rolling_heroes:
            if (character->getCharacterType() == "hero") {
                character->toggleDiceLock();
            }
            break;
        case attack_block_heroes:
            if (clickedCharacter) {
                success = character->interact(clickedCharacter, getSharedFromThis());
                if (success) {
                    clickedCharacter->setUsedDice(true);
                }
                setClickedCharacter(nullptr);
                break;
            }
            if (clickedSpell) {
                success = character->interact(clickedSpell, getSharedFromThis());
                if (success) {
                    addMana(-clickedSpell->getCost());
                }
                setClickedSpell(nullptr);
                break;
            }
            if (character->getCharacterType() == "hero") {
                if (!character->getUsedDice()) {
                    success = character->interact(std::shared_ptr<Character>(nullptr), getSharedFromThis());
                    if (success) {
                        character->setUsedDice(true);
                    } else {
                        setClickedCharacter(character);
                        setClickedSpell(nullptr);
                    }
                }
                break;
            }
            break;
        case rolling_enemies:
        case attack_block_enemies:
            break;
    }
}



void BattleController::clickSpell(const std::shared_ptr<Spell> &spell) {
    switch (state) {
        case rolling_heroes:
            break;
        case attack_block_heroes:
            if (getClickedSpell() == spell) {
                setClickedSpell(nullptr);
            } else {
                std::cout << getMana() << ", " << spell->getCost() << std::endl;
                if (getMana() >= spell->getCost()) {
                    std::cout << "done" << std::endl;
                    std::cout << getMana() << ", " << spell->getCost() << std::endl;
                    setClickedSpell(spell);
                    setClickedCharacter(nullptr);
                }
            }
            break;
        case rolling_enemies:
        case attack_block_enemies:
            break;
    }
}

}