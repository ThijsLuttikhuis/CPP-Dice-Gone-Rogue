//
// Created by thijs on 30-05-22.
//

#include <glad/glad.h>
#include <iostream>
#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

namespace DGR {

Texture2D::Texture2D(const std::string &fileName)
      : internalFormat(GL_RGBA), imageFormat(GL_RGBA), wrapS(GL_CLAMP_TO_BORDER), wrapT(GL_CLAMP_TO_BORDER),
        filterMin(GL_NEAREST), filterMag(GL_NEAREST) {

    glGenTextures(1, &id);

    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
    if (width == 0) {

        std::cerr << "texture " << fileName << " cannot be loaded!" << std::endl;
    }

    // create Texture
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);

    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}

}