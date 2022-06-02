//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm-0.9.7.1/glm/gtx/string_cast.hpp>
#include <gameobject/Hero.h>
#include "GameController.h"
#include "Window.h"

namespace DGR {

GameController::GameController(Window* window) : window(window) {
    auto* shader = new Shader();
    shader->compile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

    glm::mat4 projection = glm::ortho(0.0f, (float) window->getWidth(),
                                      (float) window->getHeight(), 0.0f,
                                      -1.0f, 1.0f);

    spriteRenderer = new SpriteRenderer(shader, projection);


    spriteRenderer->addTexture("dice_template");
    spriteRenderer->addTexture("dice_template_background");
    spriteRenderer->addTexture("dice_face_template_background");
    spriteRenderer->addTexture("dice_face_off");
    spriteRenderer->addTexture("dice_face_on");

    spriteRenderer->addTexture("rat");

    spriteRenderer->addTexture("wizard");

    spriteRenderer->addTexture("druid");

    spriteRenderer->addTexture("paladin");

    spriteRenderer->addTexture("knight");
    spriteRenderer->addTexture("knight_damage");
    spriteRenderer->addTexture("knight_shield");

    std::vector<std::string> heroNames = {"knight", "druid", "paladin", "wizard"};
    for (int x = 0; x < 7; x++) {
        glm::vec2 pos(2 * x * 24 + 32, 5 * 32);
        glm::vec2 size(32, 32);
        std::string textureName = heroNames[(int)(4.0 * rand() / RAND_MAX)];

        auto* hero = new Hero(textureName, pos, size);

        heroes.push_back(hero);
    }

    for (int x = 0; x < 7; x++) {
        glm::vec2 pos(2 * x * 12 + 32, 2 * 32);
        glm::vec2 size(16, 16);
        std::string textureName = "rat";

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

    glClearColor(0.25f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &hero : heroes) {
        hero->draw(spriteRenderer);
    }

    for (auto &hero : heroes) {
        hero->drawHover(spriteRenderer);
    }

    window->swapBuffers();
}

const std::vector<Hero*> &GameController::getHeroes() const {
    return heroes;
}

}