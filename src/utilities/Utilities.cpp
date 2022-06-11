//
// Created by thijs on 09-06-22.
//

#include <algorithm>
#include <glm/vec2.hpp>

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

}