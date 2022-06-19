//
// Created by thijs on 07-06-22.
//

#include <glm/gtc/matrix_transform.hpp>
#include <iofilemanager/YamlReader.h>
#include <scene/MainMenuScene.h>
#include <scene/SettingsScene.h>
#include "GameStateManager.h"
#include "ui/Button.h"
#include "scene/BattleScene.h"

namespace DGR {


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
    allScenes.push_back(battleScene);

    auto* mainMenuScene = new MainMenuScene(this);
    allScenes.push_back(mainMenuScene);

    auto* settingsScene = new SettingsScene(this);
    allScenes.push_back(settingsScene);
}

Window* GameStateManager::getWindow() const {
    return window;
}

void GameStateManager::update() {
    if (sceneStack.empty()) {
        for (auto & scene : allScenes) {
            if (scene->getName() == "MainMenuScene") {
                sceneStack.push_back(scene);
                break;
            }
        }
    }

    glfwPollEvents();

    t = glfwGetTime();
    dt = t - tPrev;
    tPrev = t;

    for (auto &scene : sceneStack) {
        if (scene->isEnabled()) {
            scene->update(dt);
        }
    }
}

void GameStateManager::handleMouseButton(double xPos, double yPos) {
    for (auto &scene : sceneStack) {
        if (scene->isEnabled()) {
            auto scenePos = scene->getPosition();
            double sceneXPos = xPos - scenePos.x;
            double sceneYPos = yPos - scenePos.y;
            scene->handleMouseButton(sceneXPos, sceneYPos);
        }
    }
}

void GameStateManager::handleMousePosition(double xPos, double yPos) {
    for (auto &scene : sceneStack) {
        if (scene->isEnabled()) {
            auto scenePos = scene->getPosition();
            double sceneXPos = xPos - scenePos.x;
            double sceneYPos = yPos - scenePos.y;
            scene->handleMousePosition(sceneXPos, sceneYPos);
        }
    }
}

void GameStateManager::render() {
    for (auto &scene : sceneStack) {
        //glClear(GL_DEPTH_BUFFER_BIT);

        spriteRenderer->setBaseUI(scene);
        textRenderer->setBaseUI(scene);
        scene->render(spriteRenderer, textRenderer);
    }
}

const std::vector<Scene*> &GameStateManager::getAllScenes() const {
    return allScenes;
}

const std::vector<Scene*> &GameStateManager::getSceneStack() const {
    return sceneStack;
}

void GameStateManager::addSceneToStack(const std::string &sceneName, bool disableOtherScenes) {
    for (auto &scene : allScenes) {
        if (scene->getName() == sceneName) {
            sceneStack.push_back(scene);
            scene->setIsEnabled(true);
        } else if (disableOtherScenes) {
            scene->setIsEnabled(false);
        }
    }
}

void GameStateManager::popSceneFromStack(bool enableLastSceneInStack) {
    sceneStack.pop_back();
    if (!sceneStack.empty() && enableLastSceneInStack) {
        sceneStack.back()->setIsEnabled(true);
    }
}

}
