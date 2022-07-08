//
// Created by thijs on 03-06-22.
//

#include <iostream>
#include <algorithm>
#include <utilities/Utilities.h>

#include "utilities/Constants.h"
#include "YamlReader.h"

namespace DGR {


YamlReader::YamlReader() {

    keyToFunc["characters"] = std::make_shared<YamlHandleCharacters>();

    keyToFunc["knight"] = std::make_shared<YamlHandleCharacter>("knight", stringCode::hero);
    keyToFunc["rogue"] = std::make_shared<YamlHandleCharacter>("rogue", stringCode::hero);
    keyToFunc["paladin"] = std::make_shared<YamlHandleCharacter>("paladin", stringCode::hero);
    keyToFunc["druid"] = std::make_shared<YamlHandleCharacter>("druid", stringCode::hero);
    keyToFunc["wizard"] = std::make_shared<YamlHandleCharacter>("wizard", stringCode::hero);
    keyToFunc["priest"] = std::make_shared<YamlHandleCharacter>("priest", stringCode::hero);
    keyToFunc["hunter"] = std::make_shared<YamlHandleCharacter>("hunter", stringCode::hero);

    keyToFunc["skeleton"] = std::make_shared<YamlHandleCharacter>("skeleton", stringCode::enemy);
    keyToFunc["rat"] = std::make_shared<YamlHandleCharacter>("rat", stringCode::enemy);
    keyToFunc["slime"] = std::make_shared<YamlHandleCharacter>("slime", stringCode::enemy);
    keyToFunc["dragon"] = std::make_shared<YamlHandleCharacter>("dragon", stringCode::enemy);

    keyToFunc["spell"] = std::make_shared<YamlHandleSpell>();
    keyToFunc["dice"] = std::make_shared<YamlHandleDice>();
    keyToFunc["face0"] = std::make_shared<YamlHandleFace>(0);
    keyToFunc["face1"] = std::make_shared<YamlHandleFace>(1);
    keyToFunc["face2"] = std::make_shared<YamlHandleFace>(2);
    keyToFunc["face3"] = std::make_shared<YamlHandleFace>(3);
    keyToFunc["face4"] = std::make_shared<YamlHandleFace>(4);
    keyToFunc["face5"] = std::make_shared<YamlHandleFace>(5);

    keyToFunc["hp"] = std::make_shared<YamlHandleInt>(stringCode::hp);
    keyToFunc["size"] = std::make_shared<YamlHandleInt>(stringCode::size);
    keyToFunc["name"] = std::make_shared<YamlHandleString>(stringCode::name);
    keyToFunc["cost"] = std::make_shared<YamlHandleInt>(stringCode::cost);
    keyToFunc["mod"] = std::make_shared<YamlHandleString>(stringCode::mod);
    keyToFunc["mod1"] = std::make_shared<YamlHandleString>(stringCode::mod);
    keyToFunc["damage"] = std::make_shared<YamlHandleInt>(stringCode::damage);
    keyToFunc["shield"] = std::make_shared<YamlHandleInt>(stringCode::shield);
    keyToFunc["heal"] = std::make_shared<YamlHandleInt>(stringCode::heal);
    keyToFunc["bonus health"] = std::make_shared<YamlHandleInt>(stringCode::bonus_health);
    keyToFunc["damage and self shield"] = std::make_shared<YamlHandleInt>(stringCode::damage_and_self_shield);
    keyToFunc["self shield and damage"] = std::make_shared<YamlHandleInt>(stringCode::damage_and_self_shield);
    keyToFunc["mana"] = std::make_shared<YamlHandleInt>(stringCode::mana);
    keyToFunc["dodge"] = std::make_shared<YamlHandleInt>(stringCode::dodge);
    keyToFunc["shield and mana"] = std::make_shared<YamlHandleInt>(stringCode::shield_and_mana);
    keyToFunc["mana and shield"] = std::make_shared<YamlHandleInt>(stringCode::shield_and_mana);
    keyToFunc["heal and shield"] = std::make_shared<YamlHandleInt>(stringCode::heal_and_shield);
    keyToFunc["shield and heal"] = std::make_shared<YamlHandleInt>(stringCode::heal_and_shield);
    keyToFunc["heal and mana"] = std::make_shared<YamlHandleInt>(stringCode::heal_and_mana);
    keyToFunc["mana and heal"] = std::make_shared<YamlHandleInt>(stringCode::heal_and_mana);
    keyToFunc["empty"] = std::make_shared<YamlHandleInt>(stringCode::empty);
    keyToFunc["cleanse"] = std::make_shared<YamlHandleInt>(stringCode::cleanse);

    keyToFunc["damage or shield"] = std::make_shared<YamlHandleInt>(stringCode::damage_or_shield);
    keyToFunc["heal or shield"] = std::make_shared<YamlHandleInt>(stringCode::heal_or_shield);
    keyToFunc["damage if full health"] = std::make_shared<YamlHandleInt>(stringCode::damage_if_full_health);
    keyToFunc["kill if below threshold"] = std::make_shared<YamlHandleInt>(stringCode::kill_if_below_threshold);

}

void YamlReader::readFile(const std::string &name) {
    /// read file and put all data into a string

    std::string fileName = "../src/io/characters/" + name + ".dgr";
    std::ifstream file;
    file.open(fileName);
    if (!file) { // file couldn't be opened
        std::cerr << "[YamlReader::readfile] file could not be opened" << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string worldStr = Utilities::trim(buffer.str());

    std::string word;
    size_t i = 0;
    int colonCounter = 0;
    int colonCounterMax = 8;
    auto worldStrLength = worldStr.size();

    std::vector<std::shared_ptr<YamlHandle>> objectHandle(colonCounterMax, nullptr);
    while (i < worldStrLength) {
        size_t posColon = worldStr.find(':', i);
        size_t posSemi = worldStr.find(';', i);

        if (posSemi < posColon) {
            word = Utilities::trim(worldStr.substr(i, posSemi - i));

#if DGR_DEBUG
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
            word = Utilities::trim(worldStr.substr(i, posColon - i));
#if DGR_DEBUG
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
