//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_WINDOW_H
#define DICEGONEROGUE_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputHandler.h"
#include "gameobject/Character.h"
#include "utilities/Constants.h"

namespace DGR {

class GameController;

class Window : public std::enable_shared_from_this<Window> {
private:
    int width;
    int height;

    int displayWidth{};
    int displayHeight{};

    std::unique_ptr<std::vector<bool>> keysPressed = std::make_unique<std::vector<bool>>(512, false);

    GLFWwindow* glfwWindow = nullptr;

     std::weak_ptr<GameStateManager> gameState{};

    void swapBuffers();
public:
    Window(int width, int height);

    Window() : Window(DGR_WIDTH, DGR_HEIGHT) {}

    ~Window();

    /// getters
    [[nodiscard]] std::shared_ptr<Window> getSharedFromThis();

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] bool shouldClose() const;

    /// setters
    void setGameStateManager(const std::weak_ptr<GameStateManager> &gameState_);

    /// functions
    void initialize();

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void setWindowSize(int width, int height);

    /// render
    void render();

    void handleKeyboard(int key, int action, int scanCode);
};

}

#endif //DICEGONEROGUE_WINDOW_H
