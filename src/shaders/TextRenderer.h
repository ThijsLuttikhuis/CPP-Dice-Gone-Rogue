//
// Created by thijs on 06-06-22.
//

#ifndef DICEGONEROGUE_TEXTRENDERER_H
#define DICEGONEROGUE_TEXTRENDERER_H

#include <vector>

#include "shaders/Shader.h"
#include "shaders/Texture2D.h"

namespace DGR {

class UIElement;

class TextRenderer {
private:
    static const std::vector<int> letterWidths;
    static constexpr int nLetters = 96;

     std::shared_ptr<UIElement> baseUI;

     std::shared_ptr<Shader> shader;
    unsigned int quadVAO[nLetters]{};

     std::shared_ptr<Texture2D> texture;

    [[nodiscard]] glm::vec2 displayWord(const glm::vec2 &initialTextPos, const glm::vec2 &textStart,
                                        const glm::vec2 &size, const std::vector<int> &wordVAO,
                                        const glm::vec3 &color, float alpha, float zIndex) const;

public:
    TextRenderer( std::shared_ptr<Shader> shader, glm::mat4 projection);

    ~TextRenderer();

    void setBaseUI( std::shared_ptr<DGR::UIElement> baseUI_);

    void drawText(const std::string &text, float zIndex, glm::vec2 textStart, glm::vec2 size,
                  glm::vec3 color = glm::vec3(0.0f), float alpha = 1.0f) const;

};

}

#endif //DICEGONEROGUE_TEXTRENDERER_H
