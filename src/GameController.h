//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_GAMECONTROLLER_H
#define DICEGONEROGUE_GAMECONTROLLER_H

#include <vector>

#include "gameobject/Hero.h"
#include "gameobject/Enemy.h"

namespace DGR {

class Button;

class Window;

class GameStateManager;

class GameController {
private:
    TextRenderer* textRenderer;
    SpriteRenderer* spriteRenderer;

    GameStateManager* gameState;

public:
    explicit GameController(Window* window);

    [[nodiscard]] const std::vector<Hero*> &getHeroes() const;

    [[nodiscard]] const std::vector<Enemy*> &getEnemies() const;

    [[nodiscard]] SpriteRenderer* getSpriteRenderer() const;

    void update();

    void render();

    void alignCharacterPositions(double dt);

    void clickCharacter(Character* character);

    void pressButton(Button* button);

    TextRenderer* getTextRenderer();

    void enemiesAttack();
};

}

#endif //DICEGONEROGUE_GAMECONTROLLER_H
