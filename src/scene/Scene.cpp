//
// Created by thijs on 17-06-22.
//

#include "Scene.h"

#include <utility>
#include <utilities/Constants.h>
#include <utilities/Utilities.h>
#include "ui/Button.h"
#include "GameStateManager.h"

namespace DGR {

Scene::Scene(std::string name, std::weak_ptr<GameStateManager> gameState)
      : InputHandler(std::move(gameState)),
        UIElement(std::move(name), glm::vec2(0, 0), glm::vec2(DGR_WIDTH, DGR_HEIGHT)) {}

Scene::Scene(std::string name, std::weak_ptr<GameStateManager> gameState, glm::vec2 position, glm::vec2 size,
             glm::vec3 color)
      : InputHandler(std::move(gameState)),
        UIElement(std::move(name), position, size, color) {}


std::weak_ptr<GameStateManager> Scene::getGameStateManager() const {
    return gameState;
}

bool Scene::isEnabled() const {
    return enabled;
}

void Scene::setIsEnabled(bool enabled_) {
    enabled = enabled_;
}

bool Scene::isMouseHovering(double xPos, double yPos) const {
    return Utilities::isPositionInBox(xPos, yPos, glm::vec2(0, 0), size);
}

void Scene::initialize() {
}

void Scene::onPushToStack() {
}

void Scene::onPopFromStack() {
}

void Scene::reset() {
}

void Scene::update(double dt) {
    t += dt;
}

void Scene::updateDefaults(double dt) {
    (void) dt;

    if (buttons.size() > 100) {
        std::cout << "RRRRR" << std::endl;
    }
    for (auto &button : buttons) {
        if (!button) {
            continue;
        }
        button->update(t);
    }
}

std::string Scene::message(const std::string &data) {
    return data;
}


void Scene::handleMouseButton(double xPos, double yPos) {
    handleMouseButtonDefault(xPos, yPos);
}

void Scene::handleKeyboard(int key, int action, const std::unique_ptr<std::vector<bool>> &keysPressed) {
    handleKeyboardDefault(key, action, keysPressed);
}


void Scene::handleMouseButtonDefault(double xPos, double yPos) {
    for (auto &button : buttons) {
        if (button->isPressed(xPos, yPos)) {
            pressButton(button);
        }
    }
}

void Scene::handleKeyboardDefault(int key, int action, const std::unique_ptr<std::vector<bool>> &keysPressed) {
    drawKeyPress = keysPressed->at(GLFW_KEY_LEFT_ALT);

    if (action == GLFW_RELEASE) {
        for (auto &button : buttons) {
            if (button->isKeyPressed(key)) {
                pressButton(button);
            }
        }
    }
}

bool Scene::pressDefaultButton(const std::unique_ptr<Button> &button) {
    auto gameStatePtr = std::shared_ptr<GameStateManager>(gameState);
    auto name = button->getName();
    if (name == "Settings") {
        gameStatePtr->pushSceneToStack("SettingsScene");
        return true;
    } else if (name == "Return") {
        while (gameStatePtr->popSceneFromStack());
        return true;
    } else if (name == "Close") {
        gameStatePtr->popSceneFromStack();
        return true;
    }

    return false;
}

std::unique_ptr<Button> Scene::makeDefaultButton(const std::string &name, glm::vec2 position, glm::vec2 size) {
    auto button = std::make_unique<Button>(name, position, size);

    if (name == "Settings") {
        button->setText(size.x > 30 ? "Settings" : "S");
        button->setKeyboardKey(GLFW_KEY_ESCAPE);
    } else if (name == "Close") {
        button->setText(size.x > 30 ? "Close" : "X");
        button->setKeyboardKey(GLFW_KEY_ESCAPE);
    } else if (name == "Return") {
        button->setText("Return to Main Menu");
        button->setKeyboardKey(GLFW_KEY_BACKSPACE);
    } else {
        std::cerr << "Scene::makeDefaultButton: error: default button \"" << name << "\" does not exist. exiting"
                  << std::endl;
        exit(404);
    }

    return button;
}

void Scene::renderDefaults(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer) const {
    for (auto &button : buttons) {
        button->draw(spriteRenderer, textRenderer);
        if (drawKeyPress) {
            button->drawKey(spriteRenderer, textRenderer);
        }
    }
}

void Scene::render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                   const std::unique_ptr<TextRenderer> &textRenderer) const {

    if (size.x < DGR_WIDTH || size.y < DGR_HEIGHT) {
        spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f, glm::vec2(0), size,
                                   color, 0.9f);
    }

    renderDefaults(spriteRenderer, textRenderer);
}

void Scene::pressButton(const std::unique_ptr<Button> &button) {
    std::cout << "pressed a button!" << std::endl;
    pressDefaultButton(button);
}

}
