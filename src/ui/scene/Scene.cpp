//
// Created by thijs on 17-06-22.
//

#include "Scene.h"
#include "ui/Button.h"

namespace DGR {

Scene::Scene(GameStateManager* gameState) : InputHandler(gameState) { }

bool Scene::isEnabled() const {
    return enabled;
}

void Scene::setIsEnabled(bool enabled_) {
    enabled = enabled_;
}

}
