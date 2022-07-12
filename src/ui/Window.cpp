//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <scene/BattleScene.h>

#include "Window.h"
#include "gameobject/dice/Dice.h"
#include "utilities/Constants.h"
#include "GameStateManager.h"

namespace DGR {

Window* callback_window_ptr;

void mouse_position_callback(GLFWwindow* window, double xPos, double yPos) {
    (void) window;

    callback_window_ptr->handleMousePosition(xPos, yPos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void) mods;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (action == GLFW_RELEASE) {
            callback_window_ptr->handleMouseButton(xpos, ypos);
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    (void) scancode, (void) mode;

#if DGR_DEBUG
    std::cout << "key callback" << std::endl;
#endif
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            //Breakout.Keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            //Breakout.Keys[key] = false;
            //Breakout.KeysProcessed[key] = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window;
#if DGR_DEBUG
    std::cout << "framebuffer size callback" << std::endl;
#endif

    callback_window_ptr->setWindowSize(width, height);
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height)
      : width(width), height(height) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    glfwWindow = glfwCreateWindow(width, height, "DiceGoneRogue", nullptr, nullptr);
    if (glfwWindow == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwSetWindowAspectRatio(glfwWindow, 16, 9);
    glfwSetWindowSizeLimits(glfwWindow, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(glfwWindow);

    // set callbacks
    glfwSetKeyCallback(glfwWindow, key_callback);
    glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
    glfwSetCursorPosCallback(glfwWindow, mouse_position_callback);
    glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback);
    callback_window_ptr = this;

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_ALPHA_TEST);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    const int wScreen = 1280;
    const int hScreen = 720;
    glfwSetWindowSize(glfwWindow, wScreen, hScreen);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(glfwWindow);
}

void Window::swapBuffers() {
    glfwSwapBuffers(glfwWindow);
}

Window::~Window() {
    glfwTerminate();
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

void Window::handleMouseButton(double xPos, double yPos) {
    xPos *= (double) width / displayWidth;
    yPos *= (double) height / displayHeight;

    gameState->handleMouseButton(xPos, yPos);
}

void Window::handleMousePosition(double xPos, double yPos) {
    xPos *= (double) width / displayWidth;
    yPos *= (double) height / displayHeight;

    gameState->handleMousePosition(xPos, yPos);
}

void Window::setGameStateManager(GameStateManager* gameState_) {
    gameState = gameState_;
}

void Window::setWindowSize(int displayWidth_, int displayHeight_) {
    displayWidth = displayWidth_;
    displayHeight = displayHeight_;
}

void Window::render() {
    glClearColor(0.25f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gameState->render();

    swapBuffers();

}

}