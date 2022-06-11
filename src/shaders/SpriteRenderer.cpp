//
// Created by thijs on 30-05-22.
//

#include "SpriteRenderer.h"
#include "utilities/Constants.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <filesystem>
#include <string>

namespace DGR {

SpriteRenderer::SpriteRenderer(Shader* shader, glm::mat4 projection)
      : shader(shader) {

    specialSpritesToFunction.push_back(
          std::make_pair<std::string, void (*)(SpriteRenderer* spriteRenderer, std::string texture, float zIndex,
                                               glm::vec2 position, glm::vec2 size, float rotate,
                                               glm::vec3 color, float alpha)>("box", drawBoxSprite));

    shader->use();
    shader->setInteger("sprite", 0);
    shader->setMatrix4("projection", projection);

    unsigned int VBO;
    float vertices[] = {
          // pos      // tex
          0.0f, 1.0f, 0.0f, 1.0f,
          1.0f, 0.0f, 1.0f, 0.0f,
          0.0f, 0.0f, 0.0f, 0.0f,

          0.0f, 1.0f, 0.0f, 1.0f,
          1.0f, 1.0f, 1.0f, 1.0f,
          1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &quadVAO);
}


void SpriteRenderer::drawSprite(std::string textureName, float zIndex, glm::vec2 position, glm::vec2 size,
                                float rotate, glm::vec3 color, float alpha) {

    for (auto &specialSprite : specialSpritesToFunction) {
        if (textureName == specialSprite.first) {
            specialSprite.second(this, textureName, zIndex, position, size, rotate, color, alpha);
            return;
        }
    }

    if (!textures[textureName]) {
#if PRINT_NO_TEXTURE
        std::cerr << "SpriteRenderer::drawSprite: error, no texture exist with name " << textureName << std::endl;
#endif
        textureName = "no_texture";
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader->use();
    shader->setMatrix4("model", model);
    shader->setVector3f("spriteColor", color);
    shader->setFloat("spriteAlpha", alpha);
    shader->setFloat("zIndex", zIndex);

    glActiveTexture(GL_TEXTURE0);
    textures[textureName]->bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::addTexture(const std::string &name) {
    std::string fileName = "../src/textures/" + name + ".png";
    auto texture = new Texture2D(fileName);
    textures[name] = texture;
}

void SpriteRenderer::addTexture(const std::string &fileDir, const std::string &name) {
    auto texture = new Texture2D(fileDir);
    textures[name] = texture;
}

void SpriteRenderer::addAllTexturesInDir(const std::string &dirName) {
    std::string dir = "../src/" + dirName + "/";
    auto dirIt = std::filesystem::directory_iterator(dir);
    for (const auto &entry : dirIt) {
#if DEBUG
        std::cout << entry.path() << std::endl;
#endif
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {

            addTexture(entry.path().string(), entry.path().stem().string());
        }
    }
}

void SpriteRenderer::drawBoxSprite(SpriteRenderer* spriteRenderer, std::string texture, float zIndex,
                                   glm::vec2 position, glm::vec2 size, float drawEdges, glm::vec3 color, float alpha) {

    std::string tex = "pixel";
    float left = position.x;
    float right = position.x + size.x;
    float up = position.y;
    float down = position.y + size.y;

    // draw center
        spriteRenderer->drawSprite(tex, zIndex, position, size, 0.0f, color, alpha);
    if (drawEdges > 0.001f) {
        // draw edges
        spriteRenderer->drawSprite(tex, zIndex, glm::vec2(left, up), glm::vec2(size.x, 1.0), 0.0f, color, 1.0);
        spriteRenderer->drawSprite(tex, zIndex, glm::vec2(left, down), glm::vec2(size.x+1, 1.0), 0.0f, color, 1.0);
        spriteRenderer->drawSprite(tex, zIndex, glm::vec2(left, up), glm::vec2(1.0, size.y), 0.0f, color, 1.0);
        spriteRenderer->drawSprite(tex, zIndex, glm::vec2(right, up), glm::vec2(1.0, size.y), 0.0f, color, 1.0);
    }


}

}