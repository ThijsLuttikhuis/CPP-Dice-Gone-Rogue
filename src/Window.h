//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_WINDOW_H
#define DICEGONEROGUE_WINDOW_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace DGR {

class GameController;
class Window {
private:
    int width;
    int height;

    int displayWidth;
    int displayHeight;

    GLFWwindow* glfwWindow;

    GameController* gameController;
public:
    Window(int width, int height);

    ~Window();

    int getWidth();

    int getHeight();

    void setWindowSize(int width, int height);

    bool shouldClose();

    void swapBuffers();

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void setGameController(GameController* gameController);
};

}

#endif //DICEGONEROGUE_WINDOW_H
