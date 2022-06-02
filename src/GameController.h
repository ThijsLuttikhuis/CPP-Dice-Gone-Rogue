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

    const std::vector<Hero*> &getHeroes() const;

    void update(double dt);

    void initialize();

    void render();
};

}

#endif //DICEGONEROGUE_GAMECONTROLLER_H
