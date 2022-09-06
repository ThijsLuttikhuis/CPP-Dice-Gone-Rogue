//
// Created by thijs on 20-06-22.
//

#include "BattleVictoryScene.h"
#include <GameStateManager.h>

#include <utility>
#include "ui/Button.h"

namespace DGR {

BattleVictoryScene::BattleVictoryScene(std::weak_ptr<GameStateManager> gameState)
      : Scene("BattleVictoryScene", std::move(gameState),
              glm::vec2(DGR_WIDTH * 0.1, DGR_HEIGHT * 0.1),
              glm::vec2(DGR_WIDTH * 0.8, DGR_HEIGHT * 0.8)) {

    double width = size.x;
    double height = size.y;

    double buttonWidth = width * 0.3;
    double buttonHeight = height * 0.075;
    double buttonDistance = height * 0.1;
    int i = 2;

    auto button1 = std::make_unique<Button>("Victory!",
                                            glm::vec2(width / 2 - buttonWidth / 2, i * buttonDistance),
                                            glm::vec2(buttonWidth, buttonHeight));
    button1->setText("Victory!");
    buttons.push_back(std::move(button1));
}

void BattleVictoryScene::handleMousePosition(double xPos, double yPos) {
    if (state == level_up_select) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        auto hero = gameStatePtr->getInventory()->getHeroByID(heroToLevelUp);
        auto dice = hero->getDice();
        dice->setCurrentFaceHover(false);
        dice->setCurrentFace(-1);
        for (int i = 0; i < 6; i++) {
            if (dice->getFace(i)->isMouseHovering(xPos, yPos)) {
                dice->setCurrentFace(i);
                dice->setCurrentFaceHover(true);
                break;
            }
        }
    }
}

void BattleVictoryScene::handleMouseButton(double xPos, double yPos) {
    if (!isMouseHovering(xPos, yPos) && state == get_xp_done) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        while (gameStatePtr->popSceneFromStack());
        gameStatePtr->pushSceneToStack("LevelSelectScene");
    }

    if (state == level_up_select) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        auto hero = gameStatePtr->getInventory()->getHeroByID(heroToLevelUp);
        auto dice = hero->getDice();
        for (int i = 0; i < 6; i++) {
            auto face = dice->getFace(i);
            if (face->isMouseHovering(xPos, yPos)) {
                if (face->getBonusValuePerLevel()) {
                    face->levelUp();
                    state = get_xp;
                }
            }
        }
    }

    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
        }
    }
}

void BattleVictoryScene::pressButton(const std::unique_ptr<Button> &button) {
    (void) button;
    std::cout << "pressed a button!" << std::endl;


}

void BattleVictoryScene::onPushToStack() {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    for (auto &hero : gameStatePtr->getInventory()->getHeroes()) {
        auto dice = hero->getDice();
        dice->setCurrentFace(-1);
        for (int i = 0; i < 6; i++) {
            dice->getFace(i)->setHover(false);
        }
    }

    xpPercent = 0.0;
    state = gameStatePtr->getGameProgress()->areThereItemsToGet() ? victoryGameState::get_item
                                                                  : victoryGameState::get_xp;

}


void BattleVictoryScene::onPopFromStack() {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);

    for (auto &hero : gameStatePtr->getInventory()->getHeroes()) {
        auto dice = hero->getDice();
        dice->setCurrentFace(0);
    }

}

void BattleVictoryScene::update(double dt) {
    double timeToShowXP = 2.0;
    xpPercent = xpPercent < 100.0 ? xpPercent + dt * 100 / timeToShowXP : 100.0;

    if (state == get_xp) {
        auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
        for (auto &hero : gameStatePtr->getInventory()->getHeroes()) {
            if (hero->getXPBarFill(xpPercent) >= 1.0) {
                state = level_up_select;
                heroToLevelUp = hero->getUniqueID();
                hero->levelUp();
                hero->getDice()->setCurrentFace(-1);

                auto button = getButton("Victory!");
                button.setText("Select a face to level up");
                return;
            }
        }
        if (xpPercent == 100.0) {
            state = get_xp_done;
        }
    }
}


void BattleVictoryScene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                                const std::unique_ptr<TextRenderer> &textRenderer) const {

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f, glm::vec2(0), size,
                               glm::vec3(0.2f), 0.9f);

    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
    int i;
    switch (state) {
        case get_item:

            break;
        case get_xp:
        case get_xp_done:
            i = 0;
            for (auto &hero : gameStatePtr->getInventory()->getHeroes()) {
                hero->setPosition(48 * ++i, 144);

                hero->drawHeroOnly(spriteRenderer);
                hero->drawXPBar(spriteRenderer, textRenderer, xpPercent);
            }
            break;
        case level_up_select:
            auto hero = gameStatePtr->getInventory()->getHeroByID(heroToLevelUp);

            hero->setPosition(48 * 3, 144);

            hero->drawHeroOnly(spriteRenderer);
            hero->drawLevelUp(spriteRenderer, textRenderer);

            hero->setHoverMouse(true);
            hero->drawHover(spriteRenderer, textRenderer, true);

            auto currentFace = hero->getDice()->getCurrentFace();
            if (currentFace) {
                currentFace->drawLevelUpComparison(spriteRenderer, textRenderer,
                                                   currentFace->getPosition(Dice::dice_layout_pos));
            }
    }

}

}
