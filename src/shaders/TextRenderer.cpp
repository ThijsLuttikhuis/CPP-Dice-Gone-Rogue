//
// Created by thijs on 06-06-22.
//

#include <iostream>
#include <string>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <utilities/Utilities.h>

#include "TextRenderer.h"
#include "utilities/Constants.h"
#include "ui/UIElement.h"

namespace DGR {

const std::vector<int> TextRenderer::letterWidths = {
      3, 1, 3, 5, 3, 5, 4, 1,
      2, 2, 3, 3, 1, 4, 1, 4,
      3, 1, 3, 3, 3, 3, 3, 3,
      3, 3, 1, 1, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 4, 4, 4,
      4, 3, 3, 4, 3, 5, 4, 4,
      4, 4, 4, 4, 5, 4, 4, 5,
      5, 5, 4, 2, 4, 2, 4, 4,
      3, 4, 4, 4, 4, 4, 4, 4,
      4, 2, 2, 3, 3, 5, 4, 4,
      4, 4, 3, 3, 3, 4, 4, 5,
      3, 4, 3, 0, 0, 0, 0, 0
};


TextRenderer::TextRenderer(std::shared_ptr<Shader> shader, glm::mat4 projection)
      : shader(shader) {


    std::string dir = "../src/textures/textrenderer/";
    auto dirIt = std::filesystem::directory_iterator(dir);
    for (const auto &entry : dirIt) {
#if DGR_DEBUG
        std::cout << entry.path() << std::endl;
#endif
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            texture = std::make_shared<Texture2D>(entry.path());
        }
    }

    shader->use();
    shader->setInteger("sprite", 0);
    shader->setMatrix4("projection", projection);

    unsigned int VBO;

    const int imageWidth = 480;
    const int letterWidth = 5;
    const float uvWidth = (float) letterWidth / imageWidth;
    float vertices[24 * nLetters];

    float verticesStart[] = {
          // pos      // tex
          0.0f, 1.0f, 0.0f, 1.0f,
          1.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, 0.0f, 0.0f,

          0.0f, 1.0f, 0.0f, 1.0f,
          1.0f, 1.0f, uvWidth, 1.0f,
          1.0f, 0.0f, uvWidth, 0.0f
    };
    float verticesAdd[] = {
          // pos      // tex
          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f,

          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f
    };


    for (int i = 0; i < nLetters; i++) {
        int letterStart = i * 24;

        for (int j = 0; j < 24; j++) {
            float value = verticesStart[j] + verticesAdd[j] * (float) i;
            vertices[letterStart + j] = value;
        }
    }

    for (int i = 0; i < nLetters; i++) {
        glGenVertexArrays(1, &quadVAO[i]);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesStart), &*&vertices[i * 24], GL_STATIC_DRAW);

        glBindVertexArray(quadVAO[i]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

TextRenderer::~TextRenderer() {
    for (unsigned int &i : quadVAO) {
        glDeleteVertexArrays(1, &i);
    }
}

void TextRenderer::drawText(const std::string &text, float zIndex, glm::vec2 textStart, glm::vec2 size,
                            glm::vec3 color, float alpha) const {

    glm::vec2 basePos = baseUI ? baseUI->getPosition() : glm::vec2(0, 0);
    glm::vec2 baseSize = baseUI ? baseUI->getSize() : glm::vec2(DGR_WIDTH, DGR_HEIGHT);
    glm::vec2 screenPos = textStart + basePos;

    if (screenPos.x < basePos.x || screenPos.y < basePos.y ||
        textStart.x + size.x > baseSize.x || textStart.y + size.y > baseSize.y) {

        return;
    }


    glm::vec3 white = glm::vec3(1.0f);
    glm::vec3 textColor = white;

    glm::vec2 currentTextPos = screenPos + glm::vec2(1, 1);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    // "1 damage (^single use^, ^#123456backstab^)"
    std::vector<int> wordVAO = {};
    char letterPrev = ' ';
    for (unsigned long i = 0; i < text.length(); i++) {
        char letter = text[i];

        /// add letter to word
        if (letter != '^' && letter != '#') {
            wordVAO.push_back(letter - (int) ' ');
        }

        /// if a space is found, display word
        if (letter == ' ' || letter == '^') {
            currentTextPos = displayWord(currentTextPos, screenPos, size, wordVAO, textColor, alpha, zIndex);
            wordVAO = {};
        }

        /// special text color
        if (letter == '^') {
            textColor = textColor == white ? color : white;
        } else if (letter == '#' && letterPrev == '^') {
            unsigned long colorEnd = i + 9;
            if (colorEnd < text.length()) {
                std::string hexColorString = text.substr(i + 1, 8);
                glm::vec3 hexColor = Utilities::color2Vec3(hexColorString);
                textColor = hexColor;
                i += 8;
            }
        }

        letterPrev = letter;
    }

    /// display final word
    currentTextPos = displayWord(currentTextPos, screenPos, size, wordVAO, textColor, alpha, zIndex);
}

glm::vec2 TextRenderer::displayWord(const glm::vec2 &initialTextPos, const glm::vec2 &textStart, const glm::vec2 &size,
                                    const std::vector<int> &wordVAO, const glm::vec3 &color, float alpha,
                                    float zIndex) const {

    glm::vec2 currentTextPos = initialTextPos;
    glm::vec2 letterSize(5, 7);

    /// check word width
    float wordWidth = 0.0f;
    for (auto &letterVAO : wordVAO) {
        wordWidth += letterWidths[letterVAO] + 1.0f;
    }

    /// check if the word should be put on a new line
    if (currentTextPos.x != textStart.x + 1 && currentTextPos.x + wordWidth > textStart.x + size.x) {
        currentTextPos = glm::vec2(textStart.x + 1, currentTextPos.y + letterSize.y + 1);
    }

    /// display word
    for (auto &letterVAO : wordVAO) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(currentTextPos, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(letterSize, 1.0f));

        currentTextPos += glm::vec2(letterWidths[letterVAO] + 1, 0);

        shader->use();
        shader->setMatrix4("model", model);
        shader->setVector3f("spriteColor", color);
        shader->setFloat("spriteAlpha", alpha);
        shader->setFloat("zIndex", zIndex);

        glBindVertexArray(this->quadVAO[letterVAO]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    return currentTextPos;
}

void TextRenderer::setBaseUI(std::shared_ptr<DGR::UIElement> baseUI_) {
    baseUI = baseUI_;
}

}