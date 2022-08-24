//
// Created by thijs on 30-05-22.
//

#define GLM_SWIZZLE

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
#include "utilities/Utilities.h"

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

void SpriteRenderer::drawShadowSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                                      float zIndex, const glm::vec2 &position, const glm::vec2 &size_,
                                      const spriteArgs &args) {

    glm::vec2 size = size_;

    // get args
    float height = args.count("height") ? *(float*) args.at("height") : DGR_GLOBAL_SHADOW_HEIGHT;
    float width = args.count("width") ? *(float*) args.at("width") : 1.0f;
    float alpha = args.count("alpha") ? *(float*) args.at("alpha") : DGR_GLOBAL_SHADOW_ALPHA;
    glm::vec3 color = args.count("color") ? *(glm::vec3*) args.at("color") : glm::vec3(0.0f);
    float rotate = glm::radians(args.count("rotate") ? *(float*) args.at("rotate") : 0.0f);

    float skewX = glm::radians(args.count("skewx") ? *(float*) args.at("skewx") : DGR_GLOBAL_SHADOW_XSKEW);
    float skewY = glm::radians(args.count("skewy") ? *(float*) args.at("skewy") : 0.0f);

    glm::vec2 originPosition = args.count("originposition") ? *(glm::vec2*) args.at("originposition")
                                                            : position;
    glm::vec2 originSize = args.count("originsize") ? *(glm::vec2*) args.at("originsize")
                                                    : size;

    float skewXAngle = glm::tan(skewX);
    float skewYAngle = glm::tan(skewY);

    auto &baseUI = spriteRenderer->baseUI;
    glm::vec2 basePos = baseUI ? baseUI->getPosition() : glm::vec2(0, 0);
    glm::vec2 originScreenPos = originPosition + basePos;

    // rotate
    glm::mat4 rotateModel = glm::mat4(1.0f);

    rotateModel = glm::translate(rotateModel, glm::vec3(0.5f * originSize.x, 0.5f * originSize.y, 0.0f));
    rotateModel = glm::rotate(rotateModel, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    rotateModel = glm::translate(rotateModel, glm::vec3(-0.5f * originSize.x, -0.5f * originSize.y, 0.0f));

    // skew x
    glm::mat4 skewXModel = glm::mat4(1.0f);

    if (skewXAngle != 0.0f) {
        skewXModel = glm::translate(skewXModel, glm::vec3(0.0f, originSize.y, 0.0f));
        skewXModel = glm::shearX3D(skewXModel, -skewXAngle, 0.0f);
        skewXModel = glm::translate(skewXModel, glm::vec3(0.0f, -originSize.y, 0.0f));
    }

    // skew y
    glm::mat4 skewYModel = glm::mat4(1.0f);

    if (skewYAngle != 0.0f) {
        skewYModel = glm::translate(skewYModel, glm::vec3(originSize.x, 0.0f, 0.0f));
        skewYModel = glm::shearY3D(skewYModel, -skewYAngle, 0.0f);
        skewYModel = glm::translate(skewYModel, glm::vec3(-originSize.x, 0.0f, 0.0f));
    }

    // width/height
    glm::mat4 widthheightModel = glm::mat4(1.0f);
    widthheightModel = glm::translate(widthheightModel, glm::vec3(originSize, 0.0f));
    widthheightModel = glm::scale(widthheightModel, glm::vec3(width, height, 0.0f));
    widthheightModel = glm::translate(widthheightModel, glm::vec3(-originSize, 0.0f));

    // screen space
    glm::mat4 screenModel = glm::mat4(1.0f);
    screenModel = glm::translate(screenModel, glm::vec3(originScreenPos, 0.0f));

    // local offset
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position - originPosition, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    model = screenModel * widthheightModel * skewYModel * skewXModel *rotateModel * model;

    spriteArgs shadowArgs = {{"color", &color},
                             {"alpha", &alpha},
                             {"model", &model}};

    spriteRenderer->drawSprite(texture, zIndex, position, size, shadowArgs);
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
        spriteRenderer->drawSprite(default_sprite, tex, zIndex,
                                   glm::vec2(left, up), glm::vec2(size.x, 1.0), edgeArgs);
        spriteRenderer->drawSprite(default_sprite, tex, zIndex,
                                   glm::vec2(left, down), glm::vec2(size.x + 1, 1.0), edgeArgs);
        spriteRenderer->drawSprite(default_sprite, tex, zIndex,
                                   glm::vec2(left, up), glm::vec2(1.0, size.y), edgeArgs);
        spriteRenderer->drawSprite(default_sprite, tex, zIndex,
                                   glm::vec2(right, up), glm::vec2(1.0, size.y), edgeArgs);
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
    if (screenPos.x + size.x < basePos.x || screenPos.y + size.y < basePos.y ||
        position.x > baseSize.x + 1 || position.y > baseSize.y + 1) {
#if DGR_PRINT_RENDER_OUTSIDE_SCENE
        std::cout << "Object does not fit in scene space!" << std::endl;
#endif
        return;
    }

    // get args
    glm::vec3 color = args.count("color") ? *(glm::vec3*) args.at("color") : glm::vec3(1.0f);
    float alpha = args.count("alpha") ? *(float*) args.at("alpha") : 1.0f;

    // load shadow/skew
    glm::mat4 model = glm::mat4(1.0f);
    if (args.count("model")) {
        model = *(glm::mat4*) args.at("model");

    } else {
        model = glm::translate(model, glm::vec3(screenPos, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));
    }

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
