//
// Created by thijs on 09-06-22.
//


#include "Utilities.h"
#include "Constants.h"
#include <iostream>
#include <glfw-3.3.7/include/GLFW/glfw3.h>

namespace DGR {

std::string &Utilities::spaceToUnderscore(std::string &str) {
    std::replace(str.begin(), str.end(), ' ', '_');
    return str;
}

std::string Utilities::num2OneTwoThreeString(int number) {
    std::string oneTwoThreeString;
    std::string numStr = std::to_string(number);
    if (numStr.length() > 4) {
        return "";
    }
    if (numStr.length() > 3) {
        int c = numStr[0] - '0';
        char** temp = nullptr;
        oneTwoThreeString += num2OneTwoThreeString(c) + "thousand" +
                             num2OneTwoThreeString((int) strtol(numStr.substr(1, 3).c_str(), temp, 10));
        return oneTwoThreeString;
    }
    if (numStr.length() > 2) {
        int c = numStr[0] - '0';
        char** temp = nullptr;
        oneTwoThreeString += num2OneTwoThreeString(c) + "hundred" +
                             num2OneTwoThreeString((int) strtol(numStr.substr(1, 2).c_str(), temp, 10));
        return oneTwoThreeString;
    }
    if (numStr.length() > 1) {
        int c = numStr[0] - '0';
        switch (c) {
            case 1:
                if (number == 10) {
                    return "ten";
                }
                if (number == 11) {
                    return "eleven";
                }
                if (number == 12) {
                    return "twelve";
                }
                if (number == 13) {
                    return "thirteen";
                }
                return num2OneTwoThreeString(numStr[1] - '0') + "teen";
            case 2:
                oneTwoThreeString += "twenty";
                break;
            case 3:
                oneTwoThreeString += "thirty";
                break;
            case 5:
                oneTwoThreeString += "fifty";
                break;
            default:
                oneTwoThreeString += num2OneTwoThreeString(c) + "ty";
                break;
        }
        if (number % 10 == 0) {
            return oneTwoThreeString;
        }

        oneTwoThreeString += num2OneTwoThreeString(numStr[1] - '0');

        return oneTwoThreeString;
    }

    switch (numStr[0] - '0') {
        case 0:
            return "zero";
        case 1:
            return "one";
        case 2:
            return "two";
        case 3:
            return "three";
        case 4:
            return "four";
        case 5:
            return "five";
        case 6:
            return "six";
        case 7:
            return "seven";
        case 8:
            return "eight";
        case 9:
            return "nine";
        default:
            return "error";
    }

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

std::string Utilities::checkAndRemoveDGRFileVersion(const std::string &fileContents, const std::string &fileName) {
    if (fileContents.substr(0, 13) == ".dgr.version=") {
        if (fileContents.substr(13, DGR_FILE_VERSION.size()) != DGR_FILE_VERSION) {
            std::cerr << "[Utilities::checkAndRemoveDGRFileVersion] file" << fileName
                      << " does not have the correct version (version "
                      << fileContents.substr(13, DGR_FILE_VERSION.size())
                      << ", should be " << DGR_FILE_VERSION << ")" << std::endl;
            return "";
        }
    } else {
        std::cerr << "[Utilities::checkAndRemoveDGRFileVersion] file " << fileName
                  << " does not start with \".dgr.version=\"" << std::endl;
        return "";
    }
    return fileContents.substr(13 + DGR_FILE_VERSION.size(),
                               fileContents.length() - 13 - DGR_FILE_VERSION.size());
}

std::vector<std::filesystem::path>
Utilities::getAllFileNamesInDir(const std::string &dir, const std::string &extension) {
    std::vector<std::filesystem::path> filePaths;

    auto dirIt = std::filesystem::directory_iterator(dir);
    for (const auto &entry : dirIt) {
        if (entry.path().extension() == extension) {
            filePaths.push_back(entry.path());
        }
    }

    return filePaths;
}

std::string Utilities::keyPressToName(int key) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            return "Esc";
        case GLFW_KEY_PERIOD:
            return ".";
        case GLFW_KEY_COMMA:
            return ",";

        default:
            return "NaN";
    }
}

}