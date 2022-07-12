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

std::string Scene::message(const std::string &data) {
    return data;
}

}
