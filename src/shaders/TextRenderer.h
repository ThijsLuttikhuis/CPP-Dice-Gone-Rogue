//
// Created by thijs on 06-06-22.
//

#ifndef DICEGONEROGUE_TEXTRENDERER_H
#define DICEGONEROGUE_TEXTRENDERER_H


#include "shaders/Shader.h"
#include "shaders/Texture2D.h"
#include <vector>

namespace DGR {

class TextRenderer {
private:
    static const std::vector<int> letterWidths;


    static constexpr int nLetters = 96;

    Shader* shader;
    unsigned int quadVAO[nLetters]{};

    Texture2D* texture;
public:
    TextRenderer(Shader* shader, glm::mat4 projection);

    ~TextRenderer();

    void drawText(const std::string &texture, float zIndex, glm::vec2 position, glm::vec2 size,
                  glm::vec3 color = glm::vec3(0.0f), float alpha = 1.0f);
};

}

#endif //DICEGONEROGUE_TEXTRENDERER_H
