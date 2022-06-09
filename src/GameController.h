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

    double t{};
    double dt{};
    double tPrev = 0.0;

    int animationCounter = 0;

public:
    explicit GameController(Window* window);

    /// getters
    [[nodiscard]] const std::vector<Hero*> &getHeroes() const;

    [[nodiscard]] const std::vector<Enemy*> &getEnemies() const;

    [[nodiscard]] SpriteRenderer* getSpriteRenderer() const;

    [[nodiscard]] TextRenderer* getTextRenderer() const;

    /// functions
    void update();

    void alignCharacterPositions(double dt);

    void clickCharacter(Character* character);

    void pressButton(Button* button);

    void enemiesAttack();

    void enemyAttack(int index);

    /// render
    void render();
};

}


#endif //DICEGONEROGUE_GAMECONTROLLER_H
