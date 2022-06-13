//
// Created by thijs on 03-06-22.
//

#include <iostream>
#include <algorithm>

#include "utilities/Constants.h"
#include "YamlReader.h"

namespace DGR {


YamlReader::YamlReader() {

    keyToFunc["characters"] = new YamlHandleCharacters();

    keyToFunc["knight"] = new YamlHandleCharacter("knight", stringCode::hero);
    keyToFunc["rogue"] = new YamlHandleCharacter("rogue", stringCode::hero);
    keyToFunc["paladin"] = new YamlHandleCharacter("paladin", stringCode::hero);
    keyToFunc["druid"] = new YamlHandleCharacter("druid", stringCode::hero);
    keyToFunc["wizard"] = new YamlHandleCharacter("wizard", stringCode::hero);

    keyToFunc["rat"] = new YamlHandleCharacter("rat", stringCode::enemy);
    keyToFunc["slime"] = new YamlHandleCharacter("slime", stringCode::enemy);

    keyToFunc["spell"] = new YamlHandleSpell();
    keyToFunc["dice"] = new YamlHandleDice();
    keyToFunc["face0"] = new YamlHandleFace(0);
    keyToFunc["face1"] = new YamlHandleFace(1);
    keyToFunc["face2"] = new YamlHandleFace(2);
    keyToFunc["face3"] = new YamlHandleFace(3);
    keyToFunc["face4"] = new YamlHandleFace(4);
    keyToFunc["face5"] = new YamlHandleFace(5);

    keyToFunc["hp"] = new YamlHandleInt(stringCode::hp);
    keyToFunc["size"] = new YamlHandleInt(stringCode::size);
    keyToFunc["name"] = new YamlHandleString(stringCode::name);
    keyToFunc["cost"] = new YamlHandleInt(stringCode::cost);
    keyToFunc["mod"] = new YamlHandleString(stringCode::mod);
    keyToFunc["mod1"] = new YamlHandleString(stringCode::mod);
    keyToFunc["damage"] = new YamlHandleInt(stringCode::damage);
    keyToFunc["shield"] = new YamlHandleInt(stringCode::shield);
    keyToFunc["heal"] = new YamlHandleInt(stringCode::heal);
    keyToFunc["damage and self shield"] = new YamlHandleInt(stringCode::damage_and_self_shield);
    keyToFunc["self shield and damage"] = new YamlHandleInt(stringCode::damage_and_self_shield);
    keyToFunc["mana"] = new YamlHandleInt(stringCode::mana);
    keyToFunc["dodge"] = new YamlHandleInt(stringCode::dodge);
    keyToFunc["shield and mana"] = new YamlHandleInt(stringCode::shield_and_mana);
    keyToFunc["mana and shield"] = new YamlHandleInt(stringCode::shield_and_mana);
    keyToFunc["heal and shield"] = new YamlHandleInt(stringCode::heal_and_shield);
    keyToFunc["shield and heal"] = new YamlHandleInt(stringCode::heal_and_shield);
    keyToFunc["heal and mana"] = new YamlHandleInt(stringCode::heal_and_mana);
    keyToFunc["mana and heal"] = new YamlHandleInt(stringCode::heal_and_mana);
    keyToFunc["empty"] = new YamlHandleInt(stringCode::empty);

    keyToFunc["damage or shield"] = new YamlHandleInt(stringCode::damage_or_shield);
    keyToFunc["heal or shield"] = new YamlHandleInt(stringCode::heal_or_shield);
    keyToFunc["damage if full health"] = new YamlHandleInt(stringCode::damage_if_full_health);
    keyToFunc["kill if below threshold"] = new YamlHandleInt(stringCode::kill_if_below_threshold);

}

std::string YamlReader::trim(const std::string &str, const std::string &whitespace = " \t\n\f\r\t\v") {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void YamlReader::readFile(const std::string &name) {
    /// read file and put all data into a string

    std::string fileName = "../src/iofilemanager/" + name + ".dgr";
    std::ifstream file;
    file.open(fileName);
    if (!file) { // file couldn't be opened
        std::cerr << "[YamlReader::readfile] file could not be opened" << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string worldStr = trim(buffer.str());

    std::string word;
    size_t i = 0;
    int colonCounter = 0;
    int colonCounterMax = 8;
    auto worldStrLength = worldStr.size();

    std::vector<YamlHandle*> objectHandle(colonCounterMax, nullptr);
    while (i < worldStrLength) {
        size_t posColon = worldStr.find(':', i);
        size_t posSemi = worldStr.find(';', i);

        if (posSemi < posColon) {
            word = trim(worldStr.substr(i, posSemi - i));

#if DEBUG
            std::cout << "\t" << posSemi << " ; " << word << std::endl;
#endif
            i = posSemi + 1;

            if (!word.empty()) {
                objectHandle[colonCounter] = keyToFunc.find(word) != keyToFunc.end() ? keyToFunc[word] : nullptr;
            }

            if (objectHandle[colonCounter]) {
                objectHandle[colonCounter]->handle();
                objectHandle[colonCounter - 1]->handle(objectHandle[colonCounter]);
                objectHandle[colonCounter]->reset();
                objectHandle[colonCounter] = nullptr;
            } else {
                objectHandle[colonCounter - 1]->handle(word);
            }

            colonCounter--;
        } else {
            word = trim(worldStr.substr(i, posColon - i));
#if DEBUG
            std::cout << "\t" << posColon << " : " << word << std::endl;
#endif
            i = posColon + 1;

            if (objectHandle[colonCounter]) {
                objectHandle[colonCounter - 1]->handle(objectHandle[colonCounter]);
                objectHandle[colonCounter]->reset();
            }
            objectHandle[colonCounter] = keyToFunc.find(word) != keyToFunc.end() ? keyToFunc[word] : nullptr;
            colonCounter++;
        }
    }

    if (!objectHandle[0]) {
        std::cerr << "[YamlReader::readFile] no object handle remaining" << std::endl;
    }

    handle = objectHandle[0];
}

}
