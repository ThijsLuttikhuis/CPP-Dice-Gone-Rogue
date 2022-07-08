//
// Created by thijs on 09-06-22.
//

#include <algorithm>
#include <glm/vec2.hpp>
#include <iostream>

#include "Utilities.h"

namespace DGR {

std::string &Utilities::spaceToUnderscore(std::string &str) {
    std::replace(str.begin(), str.end(), ' ', '_');
    return str;
}


bool Utilities::checkBit(unsigned int value, unsigned int pos) {
    return value & (1u << (pos));
}

bool Utilities::isPositionInBox(double xPos, double yPos, glm::vec2 boxPosition, glm::vec2 boxSize) {
    return (xPos > boxPosition.x && xPos < boxPosition.x + boxSize.x)
           && (yPos > boxPosition.y && yPos < boxPosition.y + boxSize.y);
}

float Utilities::colorHex2Float(const std::string &color_) {
    char** ptr = nullptr;
    auto hex = (double) std::strtol(color_.c_str(), ptr, 16);
    return (float) (hex / std::pow(16, (int) color_.size()));
}

glm::vec3 Utilities::color2Vec3(const std::string &color_) {
    glm::vec3 color;
    if (color_.size() == 8 && color_[0] == '0' && color_[1] == 'x') {
        for (int i = 0; i < 3; i++) {
            std::string str = color_.substr(2 * i + 2, 2);
            color[i] = colorHex2Float(str);
        }
    }
    return color;
}


std::string Utilities::trim(const std::string &str, const std::string &whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


}