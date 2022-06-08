//
// Created by thijs on 30-05-22.
//

#include <gameobject/Character.h>
#include "InputHandler.h"
#include "GameController.h"
#include <iostream>
#include "Constants.h"
#include "Button.h"

namespace DGR {

InputHandler::InputHandler(GameController* gameController) : gameController(gameController) {
    spriteRenderer = gameController->getSpriteRenderer();
    textRenderer = gameController->getTextRenderer();

    auto* button1 = new Button("leftMainButton", {268, 195}, {80, 15});
    button1->setText("2 rerolls left");
    buttons.push_back(button1);

    auto* button2 = new Button("rightMainButton", {352, 195}, {80, 15});
    button2->setText("done rolling");
    buttons.push_back(button2);
}

void InputHandler::handleMouseButton(double xPos, double yPos) {
    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            gameController->pressButton(button);
        }
    }

    auto heroes = gameController->getHeroes();
    for (auto &hero : heroes) {
        if (hero->isMouseHovering(xPos, yPos, true)) {
            gameController->clickCharacter(hero);
        }
    }

    auto enemies = gameController->getEnemies();
    for (auto &enemy : enemies) {
        if (enemy->isMouseHovering(xPos, yPos, true)) {
            gameController->clickCharacter(enemy);
        }
    }
    std::cout << xPos << ", " << yPos << std::endl;
}

void InputHandler::handleMousePosition(Character* character, double xPos, double yPos) {
    auto dice = character->getDice();
    dice->setCurrentFaceHover(dice->isMouseHovering(xPos, yPos, Dice::currentFacePos));

    if (character->isMouseHovering(xPos, yPos)) {
        character->hoverMouse(true);
    } else if (character->getHoverMouse()) {
        if (dice->isMouseHovering(xPos, yPos)) {
            dice->updateHoverMouse(xPos, yPos);
        } else {
            character->hoverMouse(false);
        }
    }
}

void InputHandler::handleMousePosition(double xPos, double yPos) {

    auto heroes = gameController->getHeroes();
    for (auto &hero : heroes) {
        handleMousePosition(hero, xPos, yPos);
    }

    auto enemies = gameController->getEnemies();
    for (auto &enemy : enemies) {
        handleMousePosition(enemy, xPos, yPos);
    }
}

void InputHandler::render() {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
    }
}

std::vector<Button*> InputHandler::getButtons() {
    return buttons;
}

}