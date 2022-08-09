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

void SpriteRenderer::drawShadowSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                                      float zIndex, const glm::vec2 &position, const glm::vec2 &size,
                                      const spriteArgs &args) {

    float height = args.count("height") ? *(float*) args.at("height") : DGR_GLOBAL_SHADOW_HEIGHT;
    float width = args.count("width") ? *(float*) args.at("width") : 1.0f;
    float alpha = args.count("alpha") ? *(float*) args.at("alpha") : DGR_GLOBAL_SHADOW_ALPHA;
    glm::vec3 color = args.count("color") ? *(glm::vec3*) args.at("color") : glm::vec3(0.0f);
    float rotate = args.count("rotate") ? *(float*) args.at("rotate") : 0.0f;

    float skewX = args.count("skewx") ? *(float*) args.at("skewx") : DGR_GLOBAL_SHADOW_XSKEW;
    float skewY = args.count("skewy") ? *(float*) args.at("skewy") : 0.0f;
    glm::vec3 skewPositionOffset = glm::vec3(args.count("skewoffset") ? *(glm::vec2*) args.at("skewoffset")
                                                                      : glm::vec2(0.0f), 0.0f);

    glm::vec3 skewPositionOffsetScaled_1 = glm::vec3(skewPositionOffset * glm::vec3((width - 1), (height - 1), 0.0f));
    glm::vec3 skewPositionOffsetScaled = glm::vec3(skewPositionOffset * glm::vec3(width, height, 0.0f));

    float skewXAngle = glm::tan(glm::radians(skewX));
    float skewYAngle = glm::tan(glm::radians(skewY));

    auto &baseUI = spriteRenderer->baseUI;
    glm::vec2 basePos = baseUI ? baseUI->getPosition() : glm::vec2(0, 0);
    glm::vec2 screenPos = position + basePos;

    glm::mat4 model = glm::mat4(1.0f);


    model = glm::translate(model, glm::vec3(screenPos, 0.0f));

    if (skewXAngle != 0.0f) {
        model = glm::translate(model, glm::vec3(skewXAngle * (skewPositionOffset.y - size.y), 0.0f, 0.0f));

        model = glm::translate(model, glm::vec3(0.0f, size.y, 0.0f));
        model = glm::shearX3D(model, -skewXAngle, 0.0f);
        model = glm::translate(model, glm::vec3(0.0f, -size.y, 0.0f));
    }

    if (skewYAngle != 0.0f) {
        model = glm::translate(model, glm::vec3(0.0f, skewYAngle * (skewPositionOffset.x - size.x), 0.0f));

        model = glm::translate(model, glm::vec3(size.x, 0.0f, 0.0f));
        model = glm::shearY3D(model, -skewYAngle, 0.0f);
        model = glm::translate(model, glm::vec3(-size.x, 0.0f, 0.0f));
    }

    model = glm::translate(model, -skewPositionOffset * glm::vec3(width - 1, height - 1, 0.0f));
    model = glm::scale(model, glm::vec3(size * glm::vec2(width, height), 1.0f));

    // skew/angle
//    model = glm::translate(model, -skewPositionOffsetScaled + glm::vec3(-screenPos.x, size.y, 0.0f));
//    model = glm::shearX3D(model, -skewXAngle, 0.0f);
//    model = glm::translate(model, skewPositionOffsetScaled + glm::vec3(screenPos.x, -size.y, 0.0f));
//
//    model = glm::translate(model, glm::vec3(size.x , -screenPos.y, 0.0f));
//    model = glm::shearY3D(model, -skewYAngle, 0.0f);
//    model = glm::translate(model, glm::vec3(-size.x, screenPos.y, 0.0f));

    spriteArgs shadowArgs = {{"color",  &color},
                             {"alpha",  &alpha},
                             {"model",  &model},
                             {"rotate", &rotate}};

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
    if (screenPos.x + size.x < basePos.x || screenPos.y + size.y < basePos.y ||
        position.x > baseSize.x + 1 || position.y > baseSize.y + 1) {
#if DGR_PRINT_RENDER_OUTSIDE_SCENE
        std::cout << "Object does not fit in scene space!" << std::endl;
#endif
        return;
    }

    // get args
    glm::vec3 color;
    float alpha;
    float rotate;

    color = args.count("color") ? *(glm::vec3*) args.at("color") : glm::vec3(1.0f);
    alpha = args.count("alpha") ? *(float*) args.at("alpha") : 1.0f;
    rotate = args.count("rotate") ? *(float*) args.at("rotate") : 0.0f;

    // load shadow/skew
    glm::mat4 model = glm::mat4(1.0f);
    if (args.count("model")) {
        model = *(glm::mat4*) args.at("model");

    } else {
        model = glm::translate(model, glm::vec3(screenPos, 0.0f));

        // rotate model
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

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
