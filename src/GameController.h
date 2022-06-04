//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_GAMECONTROLLER_H
#define DICEGONEROGUE_GAMECONTROLLER_H

#include <vector>
#include "gameobject/Hero.h"


namespace DGR {

class Window;

class GameController {
private:
    Window* window;
    SpriteRenderer* spriteRenderer;

    std::vector<Hero*> heroes;
public:
    explicit GameController(Window* window);

    [[nodiscard]] const std::vector<Hero*> &getHeroes() const;

    void initialize();

    void update(double dt);

    void render();
};

}

#endif //DICEGONEROGUE_GAMECONTROLLER_H
