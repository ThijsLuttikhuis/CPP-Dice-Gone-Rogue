//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_GAMECONTROLLER_H
#define DICEGONEROGUE_GAMECONTROLLER_H

#include <vector>
#include "gameobject/GameObject.h"
#include "Window.h"

namespace DGR {

class GameController {
private:
    Window* window;
    SpriteRenderer* spriteRenderer;

    std::vector<GameObject*> heroes;
public:
    explicit GameController(Window* window);


    void update(double dt);

    void initialize();

    void render();
};

}

#endif //DICEGONEROGUE_GAMECONTROLLER_H
