//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_WINDOW_H
#define DICEGONEROGUE_WINDOW_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace DGR {

class Window {
private:
    int width;
    int height;

    GLFWwindow* glfwWindow;

public:
    Window(int width, int height);

    ~Window();

    int getWidth();

    int getHeight();

    bool shouldClose();

    void swapBuffers();

    void handleMouseInput(double xPos, double yPos);
};

}

#endif //DICEGONEROGUE_WINDOW_H
