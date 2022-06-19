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

    GameStateManager* gameState = nullptr;

    void swapBuffers();
public:
    Window(int width, int height);

    ~Window();

    /// getters
    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] bool shouldClose() const;

    /// setters
    void setGameStateManager(GameStateManager* gameState_);

    /// functions
    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void setWindowSize(int width, int height);

    /// render
    void render();

};

}

#endif //DICEGONEROGUE_WINDOW_H
