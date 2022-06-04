//
// Created by thijs on 27-05-22.
//

#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "utilities/Window.h"
#include "GameController.h"

int main() {

    int width = 24*16;
    int height = 24*9;

    auto* window = new DGR::Window(width, height);
    auto* dgrGame = new DGR::GameController(window);
    window->setGameController(dgrGame);

    double t, dt, tPrev = 0.0;
    while (!window->shouldClose()) {

        t = glfwGetTime();

        dt = t - tPrev;
        tPrev = t;

        dgrGame->update(dt);

        dgrGame->render();
    }

    return 0;
}