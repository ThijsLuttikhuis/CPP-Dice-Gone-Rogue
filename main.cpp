//
// Created by thijs on 27-05-22.
//

#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Window.h"
#include "GameController.h"

int main() {

    int width = 512;
    int height = 288;

    auto* window = new DGR::Window(width, height);
    auto* dgrGame = new DGR::GameController(window);

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