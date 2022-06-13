//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_WINDOW_H
#define DICEGONEROGUE_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputHandler.h"
#include "gameobject/Character.h"

namespace DGR {

class GameController;

class Window {
private:
    int width;
    int height;

    int displayWidth{};
    int displayHeight{};

    GLFWwindow* glfwWindow = nullptr;

    GameController* gameController = nullptr;
    InputHandler* inputHandler = nullptr;
public:
    Window(int width, int height);

    ~Window();

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] bool shouldClose() const;

    [[nodiscard]] std::vector<Button*> getButtons() const;

    void setGameController(GameController* gameController);

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void setWindowSize(int width, int height);

    void swapBuffers();

    void render();
};

}

#endif //DICEGONEROGUE_WINDOW_H
