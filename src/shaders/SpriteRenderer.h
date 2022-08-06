//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_SPRITERENDERER_H
#define DICEGONEROGUE_SPRITERENDERER_H


#include <map>
#include <vector>

#include "shaders/Shader.h"
#include "shaders/Texture2D.h"

namespace DGR {

typedef std::map<std::string, void*> spriteArgs;

class UIElement;

class SpriteRenderer {
public:
    enum spriteType {
        default_sprite,
        shadow,
        box
    };
private:
    std::shared_ptr<Shader> shader;
    unsigned int quadVAO{};

    std::shared_ptr<UIElement> baseUI;

    std::map<std::string, std::shared_ptr<Texture2D>> textures;

    std::map<spriteType,
          void (*)(const SpriteRenderer* spriteRenderer, const std::string &texture, float zIndex,
                   const glm::vec2 &position, const glm::vec2 &size,
                   const spriteArgs &)> spriteTypeToFunction;

    static void drawBoxSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                              float zIndex, const glm::vec2 &position, const glm::vec2 &size,
                              const spriteArgs &args);

    static void drawDefaultSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                                  float zIndex, const glm::vec2 &position, const glm::vec2 &size,
                                  const spriteArgs &args);

    static void drawShadowSprite(const SpriteRenderer* spriteRenderer, const std::string &texture,
                                 float zIndex, const glm::vec2 &position, const glm::vec2 &size,
                                 const spriteArgs &args);

public:
    SpriteRenderer(const std::shared_ptr<Shader> &shader, glm::mat4 projection);

    ~SpriteRenderer();

    void addTexture(const std::string &name);

    void addAllTexturesInDir(const std::string &dirName);

    void addTexture(const std::string &fileDir, const std::string &name);

//    void drawSprite(const std::string &textureName, float zIndex, const glm::vec2 &position, const glm::vec2 &size,
//                    float rotate = 0.0f, const glm::vec3 &color = glm::vec3(1.0f),
//                    float alpha = 1.0f) const;

    bool hasTexture(const std::string &textureName);

    void setBaseUI(std::shared_ptr<DGR::UIElement> baseUI_);

    void drawSprite(spriteType type, const std::string &textureName, float zIndex, const glm::vec2 &position,
                    const glm::vec2 &size, const spriteArgs &args) const;

    void drawSprite(const std::string &textureName, float zIndex, const glm::vec2 &position,
                    const glm::vec2 &size, const spriteArgs &args) const;

    void drawSprite(spriteType type, const std::string &textureName, float zIndex, const glm::vec2 &position,
                    const glm::vec2 &size, glm::vec3 color = glm::vec3(1.0f), float alpha = 1.0f) const;

    void drawSprite(const std::string &textureName, float zIndex, const glm::vec2 &position,
                    const glm::vec2 &size, glm::vec3 color = glm::vec3(1.0f), float alpha = 1.0f) const;
};

}


#endif //DICEGONEROGUE_SPRITERENDERER_H
