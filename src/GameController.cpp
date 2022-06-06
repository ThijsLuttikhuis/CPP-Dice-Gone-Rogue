//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm-0.9.7.1/glm/gtx/string_cast.hpp>
#include <gameobject/Hero.h>
#include <shaders/TextRenderer.h>
#include "GameController.h"
#include "utilities/Window.h"
#include "iofilemanager/YamlReader.h"

namespace DGR {

GameController::GameController(Window* window) : window(window) {
    gameState = GameStateManager();

    auto* shader = new Shader();
    shader->compile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

    glm::mat4 projection = glm::ortho(0.0f, (float) window->getWidth(),
                                      (float) window->getHeight(), 0.0f,
                                      -1.0f, 1.0f);

    textRenderer = new TextRenderer(shader, projection);

    spriteRenderer = new SpriteRenderer(shader, projection);
    spriteRenderer->addAllTexturesInDir("textures");

    YamlReader yamlReaderHeroes;
    yamlReaderHeroes.readFile("heroes");
    heroes = *(std::vector<Hero*>*) yamlReaderHeroes.getData()->getFeature();

    int x = 0;
    for (auto &hero : heroes) {
        glm::vec2 pos(2 * x++ * 24 + 16, 5 * 32);
        glm::vec2 size(32, 32);

        hero->setPosition(pos);
        hero->setSize(size);
    }

    YamlReader yamlReaderEnemies;
    yamlReaderEnemies.readFile("enemies");
    enemies = *(std::vector<Enemy*>*) yamlReaderEnemies.getData()->getFeature();

    x = 0;
    for (auto &enemy : enemies) {
        glm::vec2 pos(2 * x++ * 24 + 16, 2 * 32);
        glm::vec2 size(16, 16);

        enemy->setPosition(pos);
        enemy->setSize(size);
    }

}

void GameController::update() {
    static double t, dt, tPrev = 0.0;

    glfwPollEvents();
    t = glfwGetTime();

    dt = t - tPrev;
    tPrev = t;

    alignCharacterPositions(dt);

    if (gameState.isHeroesTurn()) {

    }
}

void GameController::initialize() {

}

void GameController::render() {
    for (auto &hero : heroes) {
        hero->draw(spriteRenderer);
    }

    for (auto &enemy : enemies) {
        enemy->draw(spriteRenderer);
    }

    for (auto &hero : heroes) {
        hero->drawHover(spriteRenderer, textRenderer);
    }

    for (auto &enemy : enemies) {
        enemy->drawHover(spriteRenderer, textRenderer);
    }

}

const std::vector<Hero*> &GameController::getHeroes() const {
    return heroes;
}

const std::vector<Enemy*> &GameController::getEnemies() const {
    return enemies;
}

SpriteRenderer* GameController::getSpriteRenderer() const {
    return spriteRenderer;
}

TextRenderer* GameController::getTextRenderer() {
    return textRenderer;
}

void GameController::alignCharacterPositions(double dt) {
    const int width = window->getWidth();
    const int center = width / 2;
    const float moveSpeed = 100.0f;
    const float maxDPos = moveSpeed * (float) dt;
    const int dWidth = 16;

    int totalWidth;
    int left, up;

    // heroes
    totalWidth = 0;
    for (auto &hero : heroes) {
        if (!hero->isDead()) {
            totalWidth += dWidth + (int) hero->getSize().x;
        }
    }

    left = (int) (center * 0.7) - totalWidth / 2;
    up = 5 * 32;
    for (auto &hero : heroes) {
        if (!hero->isDead()) {
            glm::vec2 targetPos(left, up);
            glm::vec2 pos = hero->getPosition();
            glm::vec2 dPos(targetPos - pos);

            if (glm::length(dPos) > maxDPos && glm::length(dPos) < moveSpeed) {
                dPos = glm::normalize(dPos) * maxDPos;
                hero->setPosition(pos + dPos);
            } else {
                hero->setPosition(targetPos);
            }
            left += dWidth + (int) hero->getSize().x;
        } else {
            hero->setPosition(-left, -up);
        }
    }

    // enemies
    totalWidth = 0;
    for (auto &enemy : enemies) {
        if (!enemy->isDead()) {
            totalWidth += dWidth + (int) enemy->getSize().x;
        }
    }

    left = (int) (center / 0.7) - totalWidth / 2;
    up = 2 * 32;
    for (auto &enemy : enemies) {
        if (!enemy->isDead()) {
            auto targetPos = glm::vec2(left, up);
            auto pos = enemy->getPosition();
            if (pos != targetPos) {
                auto dPos = glm::vec2(targetPos - pos);
                if (glm::length(dPos) > maxDPos && glm::length(dPos) < moveSpeed) {
                    dPos = glm::normalize(dPos) * maxDPos;
                }
                enemy->setPosition(pos + dPos);
            }
            left += dWidth + (int) enemy->getSize().x;
        } else {
            enemy->setPosition(-left, -up);
        }
    }
}

void GameController::clickCharacter(Character* character) {
    character->dealDamage(character->getDice()->getFace(0));
}

void GameController::pressButton(const std::string &buttonName) {
    std::cout << "pressed a button!" << std::endl;

    if (buttonName == "button_reroll") {
        if (gameState.isHeroesTurn()) {
            reroll();
        }
    }
}

void GameController::reroll() {
    for (auto &hero : heroes) {
        hero->roll();
    }
}


}