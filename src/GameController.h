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

class GameController {
private:
    Window* window;
    SpriteRenderer* spriteRenderer;

    std::vector<Hero*> heroes;
    std::vector<Enemy*> enemies;

public:
    explicit GameController(Window* window);

    [[nodiscard]] const std::vector<Hero*> &getHeroes() const;

    [[nodiscard]] const std::vector<Enemy*> &getEnemies() const;

    void initialize();

    void update(double dt);

    void render();

};

}

#endif //DICEGONEROGUE_GAMECONTROLLER_H
