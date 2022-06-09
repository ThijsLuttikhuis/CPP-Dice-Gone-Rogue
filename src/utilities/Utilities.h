//
// Created by thijs on 09-06-22.
//

#ifndef DICEGONEROGUE_UTILITIES_H
#define DICEGONEROGUE_UTILITIES_H

#include <string>

namespace DGR {

class Utilities {
public:
    static std::string &spaceToUnderscore(std::string &str);

    static bool checkBit(unsigned int value, unsigned int pos);
};

}

#endif //DICEGONEROGUE_UTILITIES_H
