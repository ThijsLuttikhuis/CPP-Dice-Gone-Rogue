//
// Created by thijs on 17-06-22.
//

#include <utilities/Random.h>

#include <utility>
#include "BattleScene.h"
#include "ui/Button.h"
#include "GameStateManager.h"
#include "LevelSelectScene.h"

namespace DGR {

BattleScene::BattleScene(std::weak_ptr<GameStateManager> gameState) :
      Scene("BattleScene", std::move(gameState)) {

    auto button1 = std::make_unique<Button>("leftMainButton", glm::vec2(304, 232),
                                            glm::vec2(80, 15), glm::vec3(0.9));
    button1->setText("2 rerolls left");
    button1->setKeyboardKey(GLFW_KEY_COMMA);
    buttons.push_back(std::move(button1));

    auto button2 = std::make_unique<Button>("rightMainButton", glm::vec2(400, 232),
                                            glm::vec2(80, 15), glm::vec3(0.9));
    button2->setText("done rolling");
    button2->setKeyboardKey(GLFW_KEY_PERIOD);
    buttons.push_back(std::move(button2));

    auto button3 = Scene::makeDefaultButton("Settings", glm::vec2(112, 8), glm::vec2(12, 12));

    button3->setText("S");
    buttons.push_back(std::move(button3));

    auto button4 = std::make_unique<Button>("help", glm::vec2(88, 8),
                                            glm::vec2(12, 12));
    button4->setText("?");
    buttons.push_back(std::move(button4));
}

std::shared_ptr<BattleScene> BattleScene::getSharedFromThis() {
    return shared_from_this();
}

BattleScene::battleGameState BattleScene::getState() const {
    return state;
}

std::shared_ptr<Character> BattleScene::getClickedCharacter() const {
    return clickedCharacter;
}

std::shared_ptr<Spell> BattleScene::getClickedSpell() const {
    return clickedSpell;
}

int BattleScene::getRerolls() const {
    return rerolls;
}

int BattleScene::getMana() const {
    return mana;
}

std::shared_ptr<BattleLog> BattleScene::getBattleLog() const {
    return battleLog;
}

void BattleScene::setBattleLog(const std::shared_ptr<BattleLog> &battleLog_) {
    battleLog = battleLog_;
    battleLog->setBattleScene(shared_from_this());
}

void BattleScene::setClickedCharacter(std::shared_ptr<Character> clickedCharacter_) {
    clickedCharacter = std::move(clickedCharacter_);
}

void BattleScene::setClickedSpell(std::shared_ptr<Spell> clickedSpell_) {
    clickedSpell = std::move(clickedSpell_);
}

int BattleScene::reroll() {
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

void BattleScene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;

    if (pressDefaultButton(button)) {
        return;
    }

    if (button->getName() == "leftMainButton") {
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

    } else if (button->getName() == "rightMainButton") {
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

    } else if (button->getName() == "settings") {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

        gameStatePtr->pushSceneToStack("SettingsScene", true);
    } else if (button->getName() == "help") {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

        gameStatePtr->pushSceneToStack("HelpScene", false);
    }
}

void BattleScene::setHeroes(const std::vector<std::shared_ptr<Character>> &heroes_) {
    heroes = heroes_;
}

void BattleScene::setEnemies(const std::vector<std::shared_ptr<Character>> &enemies_) {
    enemies = enemies_;
}

void BattleScene::setMana(int mana_) {
    mana = mana_;
}

void BattleScene::addMana(int mana_) {
    mana += mana_;
}

bool BattleScene::areHeroesRolling() const {
    return state == rolling_heroes;
}

bool BattleScene::areEnemiesRolling() const {
    return state == rolling_enemies;
}

bool BattleScene::areHeroesAttacking() const {
    return state == attack_block_heroes;
}

bool BattleScene::areEnemiesAttacking() const {
    return state == attack_block_enemies;
}

void BattleScene::setNextGameState(bool doSaveTurn) {
    switch (state) {
        case rolling_heroes:
            for (auto &hero : heroes) {
                hero->setDiceLock(false);
            }
            rerolls = rerollsMax;
            battleLog->setState(heroes, enemies, mana);

            state = attack_block_heroes;
            updateButtons();
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
            updateButtons();
            break;
        case rolling_enemies:
            for (auto &enemy : enemies) {
                enemy->setDiceLock(false);
            }
            rerolls = rerollsMax;
            battleLog->setState(heroes, enemies, mana);

            state = attack_block_enemies;
            updateButtons();
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
            updateButtons();
            break;
    }
}

void BattleScene::updateButtons() {
    if (rerunBattle) {
        return;
    }

    for (auto &button : buttons) {
        switch (state) {
            case rolling_heroes:
                button->setEnabled(true);

                if (button->getName() == "leftMainButton") {
                    button->setText(std::to_string(getRerolls()) + " rerolls left");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("done rolling");
                }
                break;

            case attack_block_heroes:
                button->setEnabled(true);

                if (button->getName() == "leftMainButton") {
                    button->setText("undo");
                } else if (button->getName() == "rightMainButton") {
                    button->setText("done attacking");
                }
                break;

            case rolling_enemies:
            case attack_block_enemies:
                button->setEnabled(true);

                if (button->getName() == "leftMainButton") {
                    button->setEnabled(false);
                    button->setText("enemy turn...");
                } else if (button->getName() == "rightMainButton") {
                    button->setEnabled(false);
                    button->setText("enemy turn...");
                }
                break;
            default:
                break;
        }
    }
}

void BattleScene::handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos) {
    auto dice = character->getDice();
    dice->setCurrentFaceHover(dice->isMouseHovering(xPos, yPos, Dice::current_face_pos));

    if (character->isMouseHovering(xPos, yPos)) {
        character->setHoverMouse(true);
    } else if (character->getHoverMouse()) {
        if (dice->isMouseHovering(xPos, yPos)) {
            dice->updateHoverMouse(xPos, yPos);
        } else {
            character->setHoverMouse(false);
        }
    }
}

void BattleScene::handleMousePosition(double xPos, double yPos) {

    for (auto &hero : heroes) {
        handleMousePosition(hero, xPos, yPos);

        auto spell = hero->getSpell();
        if (spell) {
            spell->setHover(spell->isMouseHovering(xPos, yPos));
        }
    }

    for (auto &enemy : enemies) {
        handleMousePosition(enemy, xPos, yPos);
    }
}

void BattleScene::initialize() {
    battleLog = std::make_shared<BattleLog>(shared_from_this());
}

void BattleScene::reset() {
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

void BattleScene::update(double dt) {
    alignCharacterPositions(dt);

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

void BattleScene::enemyAttack(int index) {
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

void BattleScene::alignCharacterPositions(double dt) {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    const int width = gameStatePtr->getWindow()->getWidth();
    const int center = width / 2;
    const float moveSpeed = 100.0f;
    const float maxDPos = moveSpeed * (float) dt;
    const int dWidth = 16;
    const int maxTotalWidth = 6 * (dWidth + 32);

    int startLeft;
    int totalWidth;
    int left, up;
    bool isHeroes = false;
    for (auto &characters : {heroes, enemies}) {
        isHeroes = !isHeroes;
        totalWidth = 0;
        for (auto &character : characters) {
            if (!character->isDead()) {
                totalWidth += dWidth + (int) character->getSize().x;
            }
        }

        totalWidth = std::min(totalWidth, maxTotalWidth);

        startLeft = isHeroes ? (int) (center * 0.6) - totalWidth / 2 : (int) (center * 1.4) - totalWidth / 2;
        left = startLeft;
        up = isHeroes ? 13 * 16 : 8.5 * 16;

        for (auto &character : characters) {
            if (!character->isDead()) {
                glm::vec2 size = character->getSize();
                if ((int) size.x + left - startLeft > maxTotalWidth) {
                    std::cout << left << startLeft << maxTotalWidth << std::endl;
                    std::cout << "too many characters" << std::endl;
                    character->setPosition(-left, -up);
                    continue;
                }
                glm::vec2 targetPos(left, (float) up - size.y);
                glm::vec2 pos = character->getPosition();
                glm::vec2 dPos(targetPos - pos);

                if (glm::length(dPos) > maxDPos && glm::length(dPos) < moveSpeed) {
                    dPos = glm::normalize(dPos) * maxDPos;
                    character->setPosition(pos + dPos);
                } else {
                    character->setPosition(targetPos);
                }
                left += dWidth + (int) character->getSize().x;
            } else {
                character->setPosition(-left, -up);
            }
        }
    }
}

void BattleScene::clickSpell(const std::shared_ptr<Spell> &spell) {
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

void BattleScene::clickCharacter(const std::shared_ptr<Character> &character) {
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

void BattleScene::handleMouseButton(double xPos, double yPos) {

    handleMouseButtonDefault(xPos, yPos);

    for (auto &hero : heroes) {
        if (hero->isMouseHovering(xPos, yPos, Character::hoverType::extendedBox)) {
            clickCharacter(hero);
            return;
        }

        auto spell = hero->getSpell();
        if (spell) {
            if (spell->isMouseHovering(xPos, yPos)) {
                clickSpell(spell);
                return;
            }
        }
    }

    for (auto &enemy : enemies) {
        if (enemy->isMouseHovering(xPos, yPos, Character::hoverType::extendedBox)) {
            clickCharacter(enemy);
            return;
        }
    }

    // clicked nowhere..
//    setClickedSpell(nullptr);
//    setClickedCharacter(nullptr);
}

std::pair<std::shared_ptr<Character>, std::shared_ptr<Character>> BattleScene::getNeighbours(
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

std::string BattleScene::message(const std::string &data) {
    std::cout << data << std::endl;

    if (data.substr(0, 5) != "yes: ") {
        return data;
    }
    if (data == "yes: nextGameState") {
        setNextGameState();
    }
    return data;
}

bool BattleScene::checkVictory() {
    bool allEnemiesDead = true;
    for (auto &enemy : enemies) {
        if (!enemy->isDead()) {
            allEnemiesDead = false;
            break;
        }
    }
    if (allEnemiesDead) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        gameStatePtr->pushSceneToStack("BattleVictoryScene", true);
        updateRerunBattle(true);

        gameStatePtr->getGameProgress()->completeLevel(level, gameStatePtr->getInventory());


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

void BattleScene::setCharactersFromBattleLog() {
    auto battleLogState = battleLog->getState();
    heroes = *std::get<0>(battleLogState);
    enemies = *std::get<1>(battleLogState);
    mana = std::get<2>(battleLogState);
}

void BattleScene::rerunBattleFromStart() {
    rerunBattle = true;
    setCharactersFromBattleLog();
    auto leftMainButton = getButton("leftMainButton");
    leftMainButton.setText("Pause");
    auto rightMainButton = getButton("rightMainButton");
    rightMainButton.setText("Skip");
}

void BattleScene::updateRerunBattle(bool reset) {
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

void BattleScene::onPopFromStack() {
    reset();
}

std::vector<std::shared_ptr<Character>> BattleScene::getAliveCharacters(bool aliveHeroes) {
    std::vector<std::shared_ptr<Character>> aliveCharacters;
    for (auto &character : (aliveHeroes ? heroes : enemies)) {
        if (!character->isDead()) {
            aliveCharacters.push_back(character);
        }
    }
    return aliveCharacters;
}

void BattleScene::setEnemiesFromLevel(int selectedLevel) {
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


void BattleScene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                         const std::unique_ptr<TextRenderer> &textRenderer) const {

    spriteRenderer->drawSprite("background_catacombs", 1.0f, glm::vec2(0, 0), size,
                               glm::vec3(1.0f), 0.8f);

    for (auto &hero : heroes) {
        if (hero->getPosition().x < 0) continue;

        hero->drawShadow(spriteRenderer, textRenderer);
        hero->draw(spriteRenderer, textRenderer);
    }

    for (auto &enemy : enemies) {
        if (enemy->getPosition().x < 0) continue;

        enemy->drawShadow(spriteRenderer, textRenderer);
        enemy->draw(spriteRenderer, textRenderer);
    }

    for (auto &hero : heroes) {
        if (hero->getPosition().x < 0) continue;

        hero->drawHover(spriteRenderer, textRenderer, true);
    }

    for (auto &enemy : enemies) {
        if (enemy->getPosition().x < 0) continue;

        enemy->drawHover(spriteRenderer, textRenderer, true);
    }

    if (clickedCharacter) {
        clickedCharacter->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

    if (clickedSpell) {
        clickedSpell->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

    glm::vec2 manaPosition = glm::vec2(300, 250);
    glm::vec2 manaSize = glm::vec2(22, 24);
    glm::vec2 manaTextPosition = manaPosition + glm::vec2(8, 6);
    spriteRenderer->drawSprite("mana", 0.3f, manaPosition, manaSize,
                               glm::vec3(1.0f), 0.8f);
    textRenderer->drawText(std::to_string(mana) + "  mana", 0.2f, manaTextPosition, glm::vec2(100, 1));

    renderDefaults(spriteRenderer, textRenderer);
}

}
