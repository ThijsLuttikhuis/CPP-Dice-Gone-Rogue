//
// Created by thijs on 17-06-22.
//

#include <utilities/Random.h>
#include "BattleScene.h"
#include "ui/Button.h"
#include "GameStateManager.h"

namespace DGR {

BattleScene::BattleScene( std::shared_ptr<GameStateManager> gameState) : Scene("BattleScene", gameState) {
    auto button1 = std::make_shared<Button>("leftMainButton", glm::vec2(304, 195), glm::vec2(80, 15));
    button1->setText("2 rerolls left");
    buttons.push_back(button1);

    auto button2 = std::make_shared<Button>("rightMainButton", glm::vec2(400, 195), glm::vec2(80, 15));
    button2->setText("done rolling");
    buttons.push_back(button2);

    auto button3 = std::make_shared<Button>("settings", glm::vec2(124, 8), glm::vec2(12, 12));
    button3->setText("S");
    buttons.push_back(button3);

    auto button4 = std::make_shared<Button>("help", glm::vec2(100, 8), glm::vec2(12, 12));
    button4->setText("?");
    buttons.push_back(button4);

    attackOrder = std::make_shared<AttackOrder>(this);
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

 std::shared_ptr<AttackOrder> BattleScene::getAttackOrder() const {
    return attackOrder;
}

void BattleScene::setAttackOrder( std::shared_ptr<AttackOrder> attackOrder_) {
    attackOrder = attackOrder_;
}

void BattleScene::setClickedCharacter( std::shared_ptr<Character> clickedCharacter_) {
    clickedCharacter = clickedCharacter_;
}

void BattleScene::setClickedSpell( std::shared_ptr<Spell> clickedSpell_) {
    clickedSpell = clickedSpell_;
}

int BattleScene::reroll() {
    if (rerolls > 0) {
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
        std::cerr << "GameStateManager::reroll: no rerolls remaining!" << std::endl;
    }
#endif
    return rerolls;
}

void BattleScene::pressButton( std::shared_ptr<Button> button) {
    std::cout << "pressed a button!" << std::endl;

    if (button->getName() == "leftMainButton") {
        if (areHeroesRolling()) {
            int rerollsLeft = reroll();
            button->setText(std::to_string(rerollsLeft) + " rerolls left");
            if (rerollsLeft == 0) {
                setNextGameState();
            }
        } else if (areHeroesAttacking()) {
            getAttackOrder()->undo();
        }
    }

    if (button->getName() == "rightMainButton") {
        if (areHeroesRolling()) {
            setNextGameState();
        } else if (areHeroesAttacking()) {
            setNextGameState();
        }
    }

    if (button->getName() == "settings") {
        gameState->addSceneToStack("SettingsScene", true);
    }

    if (button->getName() == "help") {
        gameState->addSceneToStack("HelpScene", false);
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

void BattleScene::setNextGameState() {
    switch (state) {
        case rolling_heroes:
            for (auto &hero : heroes) {
                hero->setDiceLock(false);
            }
            rerolls = rerollsMax;
            attackOrder->setState(heroes, enemies, mana);

            state = attack_block_heroes;
            updateButtons();
            break;
        case attack_block_heroes:
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
            attackOrder->setState(heroes, enemies, mana);

            state = attack_block_enemies;
            updateButtons();
            break;
        case attack_block_enemies:
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

void BattleScene::handleMousePosition( std::shared_ptr<Character> character, double xPos, double yPos) {
    auto dice = character->getDice();
    dice->setCurrentFaceHover(dice->isMouseHovering(xPos, yPos, Dice::currentFacePos));

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

void BattleScene::reset() {
    rerolls = rerollsMax;
    mana = 0;
    state = rolling_enemies;
    animationCounter = 0;

    clickedCharacter = nullptr;
    clickedSpell = nullptr;

    attackOrder->reset();

    heroes = {};
    enemies = {};
}

void BattleScene::update(double dt) {
    alignCharacterPositions(dt);
    checkVictory();

    //TODO: add animations etc
    int slowDown = 30 / DGR_ANIMATION_SPEED;
    if (areEnemiesRolling()) {
        animationCounter++;
        if (animationCounter % slowDown == 0) {
            reroll();
            for (auto &enemy : enemies) {
                auto currentFace = enemy->getDice()->getCurrentFace()->getFace_();
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
    int nHeroes = heroes.size();
    int nEnemies = enemies.size();

    auto enemy = enemies[index];
    bool success;

    if (enemy->isDead()) {
        return;
    }

    // check if you can interact with yourself
    success = enemy->interact(( std::shared_ptr<Character>) nullptr, std::make_shared<BattleScene>(this));

    // check if you can interact with an ally (=enemy from their perspective)
    if (!success) {
        int mostIncomingDamage = 0;
        for (auto &otherEnemy : enemies) {
            mostIncomingDamage = std::max(mostIncomingDamage, otherEnemy->getIncomingDamage());
        }
        if (mostIncomingDamage > 0) {
            for (auto &otherEnemy : enemies) {
                if (otherEnemy->getIncomingDamage() == mostIncomingDamage) {
                    success = otherEnemy->interact(enemy, std::make_shared<BattleScene>(this));
                }
            }
        } else {
            int rng = Random::randInt(0, nEnemies - 1);
            success = enemies[rng]->interact(enemy, std::make_shared<BattleScene>(this));
        }
    }

    // finally, check if you can interact with an enemy (=hero from their perspective)
    if (!success) {
        int rng;
        while (true) {
            rng = Random::randInt(0, nHeroes - 1);
            if (!heroes[rng]->isDead()) {
                success = heroes[rng]->interact(enemy, std::make_shared<BattleScene>(this));
                break;
            }
        }
    }

    if (!success) {
        std::cerr << "GameController::enemiesAttack(): ERROR in attack logic!" << std::endl;
    }
}

void BattleScene::alignCharacterPositions(double dt) {
    const int width = gameState->getWindow()->getWidth();
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
        up = isHeroes ? 13 * 16 : 6 * 16;

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

void BattleScene::clickSpell( std::shared_ptr<Spell> spell) {
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

void BattleScene::clickCharacter( std::shared_ptr<Character> character) {
    bool success;

    switch (state) {
        case rolling_heroes:
            if (character->getCharacterType() == "hero") {
                character->toggleDiceLock();
            }
            break;
        case attack_block_heroes:
            if (clickedCharacter) {
                success = character->interact(clickedCharacter, std::make_shared<BattleScene>(this));
                if (success) {
                    clickedCharacter->setUsedDice(true);
                }
                setClickedCharacter(nullptr);
                break;
            }
            if (clickedSpell) {
                success = character->interact(clickedSpell, std::make_shared<BattleScene>(this));
                if (success) {
                    addMana(-clickedSpell->getCost());
                }
                setClickedSpell(nullptr);
                break;
            }
            if (character->getCharacterType() == "hero") {
                if (!character->getUsedDice()) {
                    success = character->interact(( std::shared_ptr<Character>) nullptr, std::make_shared<BattleScene>(this));
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
    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
            return;
        }
    }

    for (auto &hero : heroes) {
        if (hero->isMouseHovering(xPos, yPos, true)) {
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
        if (enemy->isMouseHovering(xPos, yPos, true)) {
            clickCharacter(enemy);
            return;
        }
    }

    // clicked nowhere..
    setClickedSpell(nullptr);
    setClickedCharacter(nullptr);
}

std::pair<std::shared_ptr<Character>, std::shared_ptr<Character>> BattleScene::getNeighbours( std::shared_ptr<Character> character) {
    std::pair<std::shared_ptr<Character>,  std::shared_ptr<Character>> neighbours(nullptr, nullptr);
    int nHeroes = heroes.size();
    for (int i = 0; i < nHeroes; i++) {
        if (character == heroes[i]) {
            int j = i - 1;
            while (j >= 0) {
                if (!heroes[j]->isDead()) {
                    neighbours.first = heroes[j];
                    break;
                }
                j--;
            }
            j = i + 1;
            while (j < nHeroes) {
                if (!heroes[j]->isDead()) {
                    neighbours.second = heroes[j];
                    break;
                }
                j++;
            }
            return neighbours;
        }
    }
    int nEnemies = enemies.size();
    for (int i = 0; i < nEnemies; i++) {
        if (character == enemies[i]) {
            int j = i - 1;
            while (j >= 0) {
                if (!enemies[j]->isDead()) {
                    neighbours.first = enemies[j];
                    break;
                }
                j--;
            }
            j = i + 1;
            while (j < nHeroes) {
                if (!enemies[j]->isDead()) {
                    neighbours.second = enemies[j];
                    break;
                }
                j++;
            }
            return neighbours;
        }
    }
    return neighbours;
}

void BattleScene::render( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) {
    for (auto &hero : heroes) {
        hero->draw(spriteRenderer, textRenderer);
    }

    for (auto &enemy : enemies) {
        enemy->draw(spriteRenderer, textRenderer);
    }

    for (auto &hero : heroes) {
        hero->drawHover(spriteRenderer, textRenderer);
    }

    for (auto &enemy : enemies) {
        enemy->drawHover(spriteRenderer, textRenderer);
    }

    if (clickedCharacter) {
        clickedCharacter->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

    if (clickedSpell) {
        clickedSpell->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }

    glm::vec2 manaPosition = glm::vec2(288, 216);
    glm::vec2 manaSize = glm::vec2(22, 24);
    glm::vec2 manaTextPosition = manaPosition + glm::vec2(8, 6);
    spriteRenderer->drawSprite("mana", 0.3f, manaPosition, manaSize,
                               0.0f, glm::vec3(1.0f), 0.8f);
    textRenderer->drawText(std::to_string(mana) + "  mana", 0.2f, manaTextPosition, glm::vec2(100, 1));

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

void BattleScene::checkVictory() {
    bool allEnemiesDead = true;
    for (auto &enemy : enemies) {
        if (!enemy->isDead()) {
            allEnemiesDead = false;
            break;
        }
    }
    if (allEnemiesDead) {
        gameState->addSceneToStack("BattleVictoryScene", true);
        return;
    }

    bool allHeroesDead = true;
    for (auto &hero : heroes) {
        if (!hero->isDead()) {
            allHeroesDead = false;
            break;
        }
    }
    if (allHeroesDead) {
        gameState->addSceneToStack("BattleDefeatScene", true);
        return;
    }
}

}
