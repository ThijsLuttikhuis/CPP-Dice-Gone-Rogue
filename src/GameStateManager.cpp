//
// Created by thijs on 07-06-22.
//

#include <glm/gtc/matrix_transform.hpp>
#include <iofilemanager/YamlReader.h>
#include "GameStateManager.h"
#include "ui/Button.h"
#include "ui/scene/BattleScene.h"

namespace DGR {

Window* GameStateManager::getWindow() const {
    return window;
}

void GameStateManager::update() {
    glfwPollEvents();

    t = glfwGetTime();
    dt = t - tPrev;
    tPrev = t;

    for (auto &scene : scenes) {
        scene->update(dt);
    }
}

GameStateManager::GameStateManager(Window* window) : window(window) {
    auto* shader = new Shader();
    shader->compile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

    glm::mat4 projection = glm::ortho(0.0f, (float) window->getWidth(),
                                      (float) window->getHeight(), 0.0f,
                                      -1.0f, 1.0f);

    textRenderer = new TextRenderer(shader, projection);
    spriteRenderer = new SpriteRenderer(shader, projection);
    spriteRenderer->addAllTexturesInDir("textures");

    auto* battleScene = new BattleScene(this);

    YamlReader yamlReaderHeroes;
    yamlReaderHeroes.readFile("heroes");
    auto heroes = *(std::vector<Character*>*) yamlReaderHeroes.getData()->getFeature();
    battleScene->setHeroes(heroes);

    YamlReader yamlReaderEnemies;
    yamlReaderEnemies.readFile("enemies");
    auto enemies = *(std::vector<Character*>*) yamlReaderEnemies.getData()->getFeature();
    battleScene->setEnemies(enemies);

    battleScene->reroll();

    auto attackOrder = new AttackOrder(battleScene);
    battleScene->setAttackOrder(attackOrder);

    scenes.push_back(battleScene);
}

void GameStateManager::render() {
    for (auto &scene : scenes) {
        scene->render(spriteRenderer, textRenderer);
    }
}

void GameStateManager::handleMouseButton(double xPos, double yPos) {
    for (auto &scene : scenes) {
        scene->handleMouseButton(xPos, yPos);
    }
}

void GameStateManager::handleMousePosition(double xPos, double yPos) {
    for (auto &scene : scenes) {
        scene->handleMousePosition(xPos, yPos);
    }
}

}
