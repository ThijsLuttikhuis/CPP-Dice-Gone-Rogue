//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_WINDOW_H
#define DICEGONEROGUE_WINDOW_H

#include "glad/glad.h"
#include "InputHandler.h"
#include <GLFW/glfw3.h>
#include <gameobject/Character.h>

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
    InputHandler* inputHandler;
public:
    Window(int width, int height);

    ~Window();

    [[nodiscard]] int getWidth();

    [[nodiscard]] int getHeight();

    [[nodiscard]] bool shouldClose();

    void setGameController(GameController* gameController);

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void setWindowSize(int width, int height);

    void swapBuffers();

    void handleMousePosition(DGR::Character* character, double xPos, double yPos);

    void render();
};

}

#endif //DICEGONEROGUE_WINDOW_H
