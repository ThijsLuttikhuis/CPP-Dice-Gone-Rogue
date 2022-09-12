//
// Created by thijs on 09-06-22.
//

#ifndef DICEGONEROGUE_UTILITIES_H
#define DICEGONEROGUE_UTILITIES_H

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <glm/vec2.hpp>
#include <iostream>
#include <filesystem>

namespace DGR {

class Utilities {
public:
    static std::string &spaceToUnderscore(std::string &str);

    static bool checkBit(unsigned int value, unsigned int pos);

    static bool isPositionInBox(double xPos, double yPos, glm::vec2 boxPosition, glm::vec2 boxSize);

    static glm::vec3 color2Vec3(const std::string &color_);

    static std::string vec32Color(const glm::vec3 &color_);

    static float colorHex2Float(const std::string &color_);

    static std::string float2ColorHex(const float &color_);

    static std::string trim(const std::string &str, const std::string &whitespace = " \t\n\f\r\t\v");

    static std::string checkAndRemoveDGRFileVersion(const std::string &fileContents, const std::string &fileName = "");

    static std::vector<std::filesystem::path> getAllFileNamesInDir(const std::string &dir,
                                                                   const std::string &extension = ".dgr");

    static std::string num2OneTwoThreeString(int number);

    static std::string keyPressToName(int press);

    static glm::vec2 rotateVec2(glm::vec2 vec, float rotate);

    static glm::vec2 rotateVec2(float x, float y, float rotate);

};

}

#endif //DICEGONEROGUE_UTILITIES_H
