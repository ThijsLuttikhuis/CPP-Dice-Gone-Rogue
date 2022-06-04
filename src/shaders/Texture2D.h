//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_TEXTURE2D_H
#define DICEGONEROGUE_TEXTURE2D_H

#include <string>
#include "shaders/Shader.h"

namespace DGR {

class Texture2D {
private:
    unsigned int id{};

    int width{};
    int height{};
    int channels{};

    // gl stuff
    unsigned int internalFormat;
    unsigned int imageFormat;
    unsigned int wrapS{};
    unsigned int wrapT{};
    unsigned int filterMin{};
    unsigned int filterMag{};
public:
    explicit Texture2D(const std::string &fileName);

    void bind() const;
};

}

#endif //DICEGONEROGUE_TEXTURE2D_H
