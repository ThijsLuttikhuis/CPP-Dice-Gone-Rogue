//
// Created by thijs on 30-05-22.
//

#include <gameobject/Character.h>
#include "InputHandler.h"
#include "GameController.h"
#include <iostream>
#include "Constants.h"

namespace DGR {

InputHandler::InputHandler(GameController* gameController) : gameController(gameController) {
      spriteRenderer = gameController->getSpriteRenderer();

      auto* button = new Button("button_reroll", {320,192}, {64,16});
      buttons.push_back(button);
}

void InputHandler::handleMouseButton(double xPos, double yPos) {
    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            gameController->pressButton(button->getName());
        }
    }

    auto heroes = gameController->getHeroes();
    for (auto &hero : heroes) {
        if (hero->isMouseHovering(xPos, yPos)) {
            gameController->clickCharacter(hero);
        }
    }

    auto enemies = gameController->getEnemies();
    for (auto &enemy : enemies) {
        if (enemy->isMouseHovering(xPos, yPos)) {
            gameController->clickCharacter(enemy);
        }
    }
    std::cout << xPos << ", " << yPos << std::endl;
}


void InputHandler::handleMousePosition(Character* character, double xPos, double yPos) {
    if (character->isMouseHovering(xPos, yPos)) {
        character->hoverMouse(true);
    } else if (character->getHoverMouse()) {
        auto dice = character->getDice();

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
        button->draw(spriteRenderer);
    }
}

}