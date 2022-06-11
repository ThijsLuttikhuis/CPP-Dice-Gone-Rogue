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
};

}

#endif //DICEGONEROGUE_UTILITIES_H
