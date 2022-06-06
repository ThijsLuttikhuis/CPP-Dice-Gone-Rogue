//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_GAMECONTROLLER_H
#define DICEGONEROGUE_GAMECONTROLLER_H

#include <vector>
#include "gameobject/Hero.h"
#include "gameobject/Enemy.h"

namespace DGR {

class Window;

class GameStateManager {
public:
    enum gameState {

    };
private:
    bool heroesTurn = true;

public:
    bool isHeroesTurn() {
        return heroesTurn;
    }

};

class GameController {
private:
    Window* window;
    SpriteRenderer* spriteRenderer;

    GameStateManager gameState;

    std::vector<Hero*> heroes;
    std::vector<Enemy*> enemies;

public:
    explicit GameController(Window* window);

    [[nodiscard]] const std::vector<Hero*> &getHeroes() const;

    [[nodiscard]] const std::vector<Enemy*> &getEnemies() const;

    [[nodiscard]] SpriteRenderer* getSpriteRenderer() const;

    void initialize();

    void update();

    void render();

    void alignCharacterPositions(double dt);

    void clickCharacter(Character* character);

    void pressButton(const std::string &buttonName);

    void reroll();
};

}

#endif //DICEGONEROGUE_GAMECONTROLLER_H
