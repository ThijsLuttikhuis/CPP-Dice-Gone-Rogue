//
// Created by thijs on 09-06-22.
//

#ifndef DICEGONEROGUE_UTILITIES_H
#define DICEGONEROGUE_UTILITIES_H

#include <string>
#include <glm/glm.hpp>

namespace DGR {

class Utilities {
public:
    static std::string &spaceToUnderscore(std::string &str);

    static bool checkBit(unsigned int value, unsigned int pos);

    static bool isPositionInBox(double xPos, double yPos, glm::vec2 boxPosition, glm::vec2 boxSize);

    static glm::vec3 color2Vec3(const std::string& color_);

    static float colorHex2Float(const std::string &color_);

    static std::string trim(const std::string &str, const std::string &whitespace = " \t\n\f\r\t\v");
};

}

#endif //DICEGONEROGUE_UTILITIES_H
