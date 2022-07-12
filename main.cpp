//
// Created by thijs on 27-05-22.
//


#include <utilities/Constants.h>
#include "GameStateManager.h"
#include "utilities/Random.h"
#include "ui/Window.h"

int main() {

    int width = DGR_WIDTH;
    int height = DGR_HEIGHT;

    DGR::Random::initialize(0);
    auto* window = new DGR::Window(width, height);
    auto* dgrGame = new DGR::GameStateManager(window);
    window->setGameStateManager(dgrGame);

    while (!window->shouldClose()) {
        dgrGame->update();
        window->render();
    }

    return 0;
}