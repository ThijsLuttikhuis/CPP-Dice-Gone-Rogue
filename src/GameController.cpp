//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm-0.9.7.1/glm/gtx/string_cast.hpp>
#include <gameobject/Hero.h>
#include "GameController.h"

namespace DGR {

GameController::GameController(Window* window) : window(window) {
    auto* shader = new Shader();
    shader->compile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

    glm::mat4 projection = glm::ortho(0.0f, (float) window->getWidth(),
                                      (float) window->getHeight(), 0.0f,
                                      -1.0f, 1.0f);

    spriteRenderer = new SpriteRenderer(shader, projection);

    spriteRenderer->addTexture("wizard");
    spriteRenderer->addTexture("druid");
    spriteRenderer->addTexture("knight");
    spriteRenderer->addTexture("diceTemplate");

    for (int x = 0; x < 5; x++) {
        glm::vec2 pos(2 * x * 32 + 32, 6 * 32);
        glm::vec2 size(32, 32);
        std::string textureName = (float) rand() / RAND_MAX > 0.66 ? "knight" :
                                  (float) rand() / RAND_MAX > 0.66 ? "druid" :
                                  "wizard";

        auto* hero = new Hero(textureName, pos, size);

        heroes.push_back(hero);
    }

}

void GameController::update(double dt) {
    glfwPollEvents();
    //std::cout << dt << std::endl;
}

void GameController::initialize() {

}

void GameController::render() {

    glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &hero : heroes) {
        hero->draw(spriteRenderer);
    }

    window->swapBuffers();
}

}