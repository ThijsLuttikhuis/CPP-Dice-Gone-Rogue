//
// Created by thijs on 09-06-22.
//

#include <algorithm>

#include "Utilities.h"

namespace DGR {

std::string &Utilities::spaceToUnderscore(std::string &str) {
    std::replace(str.begin(), str.end(), ' ', '_');
    return str;
}


bool Utilities::checkBit(unsigned int value, unsigned int pos) {
    return value & (1u << (pos));
}


}