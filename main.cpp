//
// Created by thijs on 27-05-22.
//


#include <utilities/Constants.h>
#include "GameStateManager.h"
#include "utilities/Random.h"
#include "ui/Window.h"

int main() {

    DGR::Random::initialize();
    std::shared_ptr<DGR::Window> window = std::make_shared<DGR::Window>();
    std::shared_ptr<DGR::GameStateManager> dgrGame = std::make_shared<DGR::GameStateManager>(window);

    dgrGame->initialize();
    window->setGameStateManager(dgrGame->getSharedFromThis());
    window->initialize();

//    for (int i = 0; i < 100; i++) {
//        std::cout << DGR::GameProgress::levelToEnemyStrength(i) << std::endl;
//    }

    while (!window->shouldClose()) {
        dgrGame->update();
        window->render();
    }

    dgrGame.reset();
    window.reset();

    return 0;
}
