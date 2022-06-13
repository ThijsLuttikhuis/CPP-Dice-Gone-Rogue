//
// Created by thijs on 27-05-22.
//


#include "GameController.h"
#include "utilities/Random.h"
#include "ui/Window.h"

int main() {

    int width = 32*16;
    int height = 32*9;

    DGR::Random::initialize(0);
    auto* window = new DGR::Window(width, height);
    auto* dgrGame = new DGR::GameController(window);
    window->setGameController(dgrGame);

    while (!window->shouldClose()) {
        dgrGame->update();
        window->render();
    }

    return 0;
}