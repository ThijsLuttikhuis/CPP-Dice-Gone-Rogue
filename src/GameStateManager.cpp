//
// Created by thijs on 07-06-22.
//

#include <glm/gtc/matrix_transform.hpp>
#include <iofilemanager/YamlReader.h>
#include <scene/MainMenuScene.h>
#include <scene/SettingsScene.h>
#include <scene/BattleVictoryScene.h>
#include <scene/BattleDefeatScene.h>
#include <scene/CharacterSelectScene.h>
#include "GameStateManager.h"
#include "ui/Button.h"
#include "scene/BattleScene.h"

namespace DGR {

GameStateManager::GameStateManager(Window* window) : window(window) {
    auto* shader = new Shader();
    shader->compile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

    glm::mat4 projection = glm::ortho(0.0f, (float) window->getWidth(), (float) window->getHeight(),
                                      0.0f, -1.0f, 1.0f);

    textRenderer = new TextRenderer(shader, projection);
    spriteRenderer = new SpriteRenderer(shader, projection);
    spriteRenderer->addAllTexturesInDir("textures");

    YamlReader yamlReaderHeroes;
    yamlReaderHeroes.readFile("heroes");
    allHeroes = *(std::vector<Character*>*) yamlReaderHeroes.getData()->getFeature();

    YamlReader yamlReaderEnemies;
    yamlReaderEnemies.readFile("enemies");
    allEnemies = *(std::vector<Character*>*) yamlReaderEnemies.getData()->getFeature();

    auto battleScene = new BattleScene(this);
    allScenes.push_back(battleScene);

    auto mainMenuScene = new MainMenuScene(this);
    allScenes.push_back(mainMenuScene);

    auto settingsScene = new SettingsScene(this);
    allScenes.push_back(settingsScene);

    auto battleVictoryScene = new BattleVictoryScene(this);
    allScenes.push_back(battleVictoryScene);

    auto battleDefeatScene = new BattleDefeatScene(this);
    allScenes.push_back(battleDefeatScene);

    auto characterSelectScene = new CharacterSelectScene(this);
    allScenes.push_back(characterSelectScene);
}

Window* GameStateManager::getWindow() const {
    return window;
}

void GameStateManager::update() {
    if (sceneStack.empty()) {
        for (auto &scene : allScenes) {
            if (scene->getName() == "MainMenuScene") {
                scene->setIsEnabled(true);
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
        glClear(GL_DEPTH_BUFFER_BIT);

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

bool GameStateManager::addSceneToStack(const std::string &sceneName, bool disableOtherScenes) {
    bool sceneFound = false;
    for (auto &scene : allScenes) {
        if (scene->getName() == sceneName) {
            sceneFound = true;
            sceneStack.push_back(scene);
            scene->setIsEnabled(true);
            break;
        }
    }
    if (disableOtherScenes && sceneFound) {
        for (auto &scene : sceneStack) {
            scene->setIsEnabled(scene->getName() == sceneName);
        }
    }

    return sceneFound;
}

bool GameStateManager::popSceneFromStack(bool enableLastSceneInStack) {
    if (sceneStack.empty()) {
        return false;
    }
    sceneStack.pop_back();
    if (!sceneStack.empty() && enableLastSceneInStack) {
        sceneStack.back()->setIsEnabled(true);
    }

    return true;
}

Scene* GameStateManager::getScene(const std::string &sceneName, bool onlySceneStack) const {
    auto scenes = onlySceneStack ? sceneStack : allScenes;
    for (auto &scene : scenes) {
        if (scene->getName() == sceneName) {
            return scene;
        }
    }
    return nullptr;
}

const std::vector<Character*> &GameStateManager::getAllHeroes() const {
    return allHeroes;
}

const std::vector<Character*> &GameStateManager::getAllEnemies() {
    return allEnemies;
}

}
