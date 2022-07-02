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

GameStateManager::GameStateManager(std::shared_ptr<Window> window) : window(window) {
    auto shader = std::make_shared< Shader>();
    shader->compile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

    glm::mat4 projection = glm::ortho(0.0f, (float) window->getWidth(), (float) window->getHeight(),
                                      0.0f, -1.0f, 1.0f);

    textRenderer = std::make_shared<TextRenderer>(shader, projection);
    spriteRenderer = std::make_shared<SpriteRenderer>(shader, projection);
    spriteRenderer->addAllTexturesInDir("textures");

    YamlReader yamlReaderHeroes;
    yamlReaderHeroes.readFile("heroes");
    allHeroes = *(std::vector<std::shared_ptr<Character>>*) yamlReaderHeroes.getData()->getFeature();

    int size = (int) allHeroes.size();
    for (int i = 0; i < size; i++) {
        allHeroes.push_back(allHeroes[i]->makeCopy(false));
    }
    for (int i = 0; i < size; i++) {
        allHeroes.push_back(allHeroes[i]->makeCopy(false));
    }

    YamlReader yamlReaderEnemies;
    yamlReaderEnemies.readFile("enemies");
    allEnemies = *(std::vector<std::shared_ptr<Character>>*) yamlReaderEnemies.getData()->getFeature();

    auto battleScene = std::make_shared<BattleScene>(this);
    allScenes.push_back(battleScene);

    auto mainMenuScene = std::make_shared<MainMenuScene>(this);
    allScenes.push_back(mainMenuScene);

    auto settingsScene = std::make_shared<SettingsScene>(this);
    allScenes.push_back(settingsScene);

    auto battleVictoryScene = std::make_shared< BattleVictoryScene>(this);
    allScenes.push_back(battleVictoryScene);

    auto battleDefeatScene = std::make_shared< BattleDefeatScene>(this);
    allScenes.push_back(battleDefeatScene);

    auto characterSelectScene = std::make_shared< CharacterSelectScene>(this);
    allScenes.push_back(characterSelectScene);
}

std::shared_ptr<Window> GameStateManager::getWindow() const {
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

    for (auto &onScreenMessage : onScreenMessages) {
        onScreenMessage->update(dt);
    }

    onScreenMessages.erase(std::remove_if(onScreenMessages.begin(), onScreenMessages.end(),
                                          [](std::shared_ptr<OnScreenMessage>& message) {
                                              return message->getDuration() < 0.0;
                                          }), onScreenMessages.end());

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

const std::vector<std::shared_ptr<Scene>> &GameStateManager::getAllScenes() const {
    return allScenes;
}

const std::vector<std::shared_ptr<Scene>> &GameStateManager::getSceneStack() const {
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

std::shared_ptr<Scene> GameStateManager::getScene(const std::string &sceneName, bool onlySceneStack) const {
    auto scenes = onlySceneStack ? sceneStack : allScenes;
    for (auto &scene : scenes) {
        if (scene->getName() == sceneName) {
            return scene;
        }
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Character>> &GameStateManager::getAllHeroes() const {
    return allHeroes;
}

const std::vector<std::shared_ptr<Character>> &GameStateManager::getAllEnemies() const {
    return allEnemies;
}

void GameStateManager::addOnScreenMessage(std::shared_ptr<OnScreenMessage> message) {
    onScreenMessages.push_back(message);
}

void GameStateManager::addOnScreenMessage(const std::string& message) {
    auto onScreenMessage = std::make_shared< OnScreenMessage>(message);
    onScreenMessages.push_back(onScreenMessage);
}

void GameStateManager::render() {
    for (auto &scene : sceneStack) {
        glClear(GL_DEPTH_BUFFER_BIT);

        spriteRenderer->setBaseUI(scene);
        textRenderer->setBaseUI(scene);
        scene->render(spriteRenderer, textRenderer);
    }

    spriteRenderer->setBaseUI(nullptr);
    textRenderer->setBaseUI(nullptr);

    for (auto &message : onScreenMessages) {
        glClear(GL_DEPTH_BUFFER_BIT);
        message->draw(spriteRenderer, textRenderer);
    }
}

}
