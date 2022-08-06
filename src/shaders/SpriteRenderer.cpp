//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <filesystem>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <map>
#include <utility>

#include "SpriteRenderer.h"
#include "utilities/Constants.h"
#include "ui/UIElement.h"

namespace DGR {

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Shader> &shader, glm::mat4 projection)
      : shader(shader) {


    spriteTypeToFunction[spriteType::default_sprite] = drawDefaultSprite;
    spriteTypeToFunction[spriteType::box] = drawBoxSprite;
    spriteTypeToFunction[spriteType::shadow] = drawShadowSprite;

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

void SpriteRenderer::addTexture(const std::string &name) {
    std::string fileName = "../src/textures/" + name + ".png";
    auto texture = std::make_shared<Texture2D>(fileName);
    textures[name] = texture;
}

void SpriteRenderer::addTexture(const std::string &fileDir, const std::string &name) {
    auto texture = std::make_shared<Texture2D>(fileDir);
    textures[name] = texture;
}

void SpriteRenderer::addAllTexturesInDir(const std::string &dirName) {
    std::string dir = "../src/" + dirName + "/";
    auto dirIt = std::filesystem::directory_iterator(dir);
    for (const auto &entry : dirIt) {
#if DGR_DEBUG
        std::cout << entry.path() << std::endl;
#endif
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {

            addTexture(entry.path(), entry.path().stem());
        }
    }
}

bool SpriteRenderer::hasTexture(const std::string &textureName) {
    return textures.find(textureName) != textures.end();
}

void SpriteRenderer::setBaseUI(std::shared_ptr<DGR::UIElement> baseUI_) {
    baseUI = std::move(baseUI_);
}


//
//void SpriteRenderer::drawSprite(const std::string &textureName_, float zIndex,
//                                const glm::vec2 &position, const glm::vec2 &size,
//                                float rotate, const glm::vec3 &color, float alpha) const {
//
//    std::string textureName;
//
//    // draw shadow
//    bool shadow = false;
//    if (textureName_.length() > 8 && textureName_.substr(0, 8) == "shadow: ") {
//        textureName = textureName_.substr(8, textureName_.length() - 8);
//        shadow = true;
//    } else {
//        textureName = textureName_;
//    }
//
//    for (auto &specialSprite : specialSpritesToFunction) {
//        if (textureName == specialSprite.first) {
//            specialSprite.second(this, textureName, zIndex, position, size, rotate,
//                                 color, alpha);
//            return;
//        }
//    }
//
//
//    glm::vec2 basePos = baseUI ? baseUI->getPosition() : glm::vec2(0, 0);
//    glm::vec2 baseSize = baseUI ? baseUI->getSize() : glm::vec2(DGR_WIDTH, DGR_HEIGHT);
//    glm::vec2 screenPos = position + basePos;
//    if (screenPos.x < basePos.x || screenPos.y < basePos.y ||
//        position.x + size.x > baseSize.x + 1 || position.y + size.y > baseSize.y + 1) {
//#if DGR_PRINT_RENDER_OUTSIDE_SCENE
//        std::cout << "Object does not fit in scene space!" << std::endl;
//#endif
//        return;
//    }
//
//    glm::mat4 model = glm::mat4(1.0f);
//
//
//    model = glm::translate(model, glm::vec3(screenPos, 0.0f));
//
//    if (shadow) {
//        float shadowAngle = glm::tan(glm::radians(rotate));
//        float shadowHeight = alpha;
//
//        // skew/angle
//        model = glm::translate(model, glm::vec3(position.x, size.y, 0.0f));
//        model = glm::shearX3D(model, -shadowAngle, 0.0f);
//        model = glm::translate(model, glm::vec3(-position.x, -size.y, 0.0f));
//
//        // height
//        model = glm::translate(model, glm::vec3(0.0f, size.y, 0.0f));
//        model = glm::scale(model, glm::vec3(1.0f, shadowHeight, 0.0f));
//        model = glm::translate(model, glm::vec3(0.0f, -size.y, 0.0f));
//    }
//
//    model = glm::scale(model, glm::vec3(size, 1.0f));
//
//    shader->use();
//    shader->setMatrix4("model", model);
//    shader->setVector3f("spriteColor", shadow ? glm::vec3(0.0f) : color);
//    shader->setFloat("spriteAlpha", shadow ? 0.35f : alpha);
//    shader->setFloat("zIndex", zIndex);
//
//    glActiveTexture(GL_TEXTURE0);
//
//    if (textures.find(textureName) != textures.end()) {
//        textures.at(textureName)->bind();
//    } else {
//#if DGR_PRINT_NO_TEXTURE
//        std::cerr << "SpriteRenderer::drawSprite: error, no texture exist with name " << textureName << std::endl;
//#endif
//        textures.at("no_texture")->bind();
//    }
//
//    glBindVertexArray(this->quadVAO);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glBindVertexArray(0);
//}

void SpriteRenderer::drawShadowSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                                      float zIndex, const glm::vec2 &position, const glm::vec2 &size,
                                      const std::map<std::string, void*> &args) {

//
//    float shadowAngle = glm::tan(glm::radians(rotate));
//    float shadowHeight = alpha;
//
//    // skew/angle
//    model = glm::translate(model, glm::vec3(position.x, size.y, 0.0f));
//    model = glm::shearX3D(model, -shadowAngle, 0.0f);
//    model = glm::translate(model, glm::vec3(-position.x, -size.y, 0.0f));
//
//    // height
//    model = glm::translate(model, glm::vec3(0.0f, size.y, 0.0f));
//    model = glm::scale(model, glm::vec3(1.0f, shadowHeight, 0.0f));
//    model = glm::translate(model, glm::vec3(0.0f, -size.y, 0.0f));
}


void SpriteRenderer::drawBoxSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                                   float zIndex, const glm::vec2 &position, const glm::vec2 &size,
                                   const std::map<std::string, void*> &args) {
    (void) texture;

    // get box corners
    std::string tex = "pixel";
    float left = position.x;
    float right = position.x + size.x;
    float up = position.y;
    float down = position.y + size.y;

    // draw center
    drawDefaultSprite(spriteRenderer, tex, zIndex, position, size, args);
    if (args.count("edgeAlpha")) {
        std::map<std::string, void*> edgeArgs;

        edgeArgs["alpha"] = args.at("edgeAlpha");
        edgeArgs["color"] = args.at("color");
        // draw edges
        spriteRenderer->drawSprite(default_sprite, tex, zIndex, glm::vec2(left, up), glm::vec2(size.x, 1.0), edgeArgs);
        spriteRenderer->drawSprite(default_sprite, tex, zIndex, glm::vec2(left, down), glm::vec2(size.x + 1, 1.0),
                                   edgeArgs);
        spriteRenderer->drawSprite(default_sprite, tex, zIndex, glm::vec2(left, up), glm::vec2(1.0, size.y), edgeArgs);
        spriteRenderer->drawSprite(default_sprite, tex, zIndex, glm::vec2(right, up), glm::vec2(1.0, size.y), edgeArgs);
    }
}

void SpriteRenderer::drawDefaultSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                                       float zIndex, const glm::vec2 &position, const glm::vec2 &size,
                                       const std::map<std::string, void*> &args) {

    // move position from screen space to scene space
    auto &baseUI = spriteRenderer->baseUI;
    glm::vec2 basePos = baseUI ? baseUI->getPosition() : glm::vec2(0, 0);
    glm::vec2 baseSize = baseUI ? baseUI->getSize() : glm::vec2(DGR_WIDTH, DGR_HEIGHT);
    glm::vec2 screenPos = position + basePos;

    // check if object fits in scene space window
    if (screenPos.x < basePos.x || screenPos.y < basePos.y ||
        position.x + size.x > baseSize.x + 1 || position.y + size.y > baseSize.y + 1) {
#if DGR_PRINT_RENDER_OUTSIDE_SCENE
        std::cout << "Object does not fit in scene space!" << std::endl;
#endif
        return;
    }

    // get args
    glm::vec3 color;
    float alpha;
    glm::mat4 model;

    color = args.count("color") ? *(glm::vec3*) args.at("color") : glm::vec3(1.0f);
    alpha = args.count("alpha") ? *(float*) args.at("alpha") : 1.0f;
    model = args.count("model") ? *(glm::mat4*) args.at("model") : glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(screenPos, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // apply shader variables
    auto &shader = spriteRenderer->shader;
    spriteRenderer->shader->use();
    shader->setMatrix4("model", model);
    shader->setVector3f("spriteColor", color);
    shader->setFloat("spriteAlpha", alpha);
    shader->setFloat("zIndex", zIndex);

    // set texture
    glActiveTexture(GL_TEXTURE0);
    auto &textures = spriteRenderer->textures;
    if (textures.find(texture) != textures.end()) {
        textures.at(texture)->bind();
    } else {
#if DGR_PRINT_NO_TEXTURE
        std::cerr << "SpriteRenderer::drawSprite: error, no texture exist with name " << textureName << std::endl;
#endif
        textures.at("no_texture")->bind();
    }

    // draw texture
    glBindVertexArray(spriteRenderer->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(const std::string &textureName, float zIndex, const glm::vec2 &position,
                                const glm::vec2 &size, const std::map<std::string, void*> &args) const {

    drawSprite(default_sprite, textureName, zIndex, position, size, args);
}

void SpriteRenderer::drawSprite(const std::string &textureName, float zIndex, const glm::vec2 &position,
                                const glm::vec2 &size, glm::vec3 color, float alpha) const {

    drawSprite(default_sprite, textureName, zIndex, position, size, color, alpha);
}


void SpriteRenderer::drawSprite(spriteType type, const std::string &textureName, float zIndex,
                                const glm::vec2 &position, const glm::vec2 &size,
                                glm::vec3 color, float alpha) const {

    std::map<std::string, void*> args;
    args["color"] = (void*) &color;
    args["alpha"] = (void*) &alpha;

    drawSprite(type, textureName, zIndex, position, size, args);
}

void SpriteRenderer::drawSprite(spriteType type, const std::string &textureName, float zIndex,
                                const glm::vec2 &position, const glm::vec2 &size,
                                const std::map<std::string, void*> &args) const {

    if (spriteTypeToFunction.find(type) != spriteTypeToFunction.end()) {
        spriteTypeToFunction.at(type)(this, textureName, zIndex, position, size, args);
    } else {
        std::cerr << "SpriteRenderer::drawSprite: error, spriteType not found.." << std::endl;
    }
}

}
