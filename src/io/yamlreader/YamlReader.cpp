//
// Created by thijs on 03-06-22.
//

#include <iostream>
#include <algorithm>
#include <utilities/Utilities.h>

#include "utilities/Constants.h"
#include "YamlReader.h"
#include "YamlReaderCharacters.h"
#include "YamlReaderItems.h"

namespace DGR {


YamlReader::YamlReader() {

    keyToFunc["characters"] = std::make_shared<YamlHandleCharacters>();

    std::vector<std::string> heroNames = {"knight", "rogue", "paladin", "druid", "wizard", "priest", "ranger"};
    for (const auto& heroName : heroNames) {
        keyToFunc[heroName] = std::make_shared<YamlHandleCharacter>(heroName, stringCode::hero);
    }

    std::vector<std::string> enemyNames = {"skeleton", "rat", "slime", "dragon"};
    for (const auto& enemyName : enemyNames) {
        keyToFunc[enemyName] = std::make_shared<YamlHandleCharacter>(enemyName, stringCode::enemy);
    }

    std::vector<std::string> itemNames = {"ring of vigor", "ring of courage", "thornmail"};
    for (const auto& itemName : itemNames) {
        keyToFunc[itemName] = std::make_shared<YamlHandleItem>(itemName, stringCode::item);
    }

    keyToFunc["spell"] = std::make_shared<YamlHandleSpell>();
    keyToFunc["dice"] = std::make_shared<YamlHandleDice>();

    for (int i = 0; i < 6; i++) {
        keyToFunc["face" + std::to_string(i)] = std::make_shared<YamlHandleFace>(i);
    }

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
    keyToFunc["damage and mana"] = std::make_shared<YamlHandleInt>(stringCode::damage_and_mana);
    keyToFunc["mana and damage"] = std::make_shared<YamlHandleInt>(stringCode::damage_and_mana);
    keyToFunc["empty"] = std::make_shared<YamlHandleInt>(stringCode::empty);
    keyToFunc["cleanse"] = std::make_shared<YamlHandleInt>(stringCode::cleanse);
    keyToFunc["bonus"] = std::make_shared<YamlHandleInt>(stringCode::face_bonus);

    keyToFunc["damage or shield"] = std::make_shared<YamlHandleInt>(stringCode::damage_or_shield);
    keyToFunc["heal or shield"] = std::make_shared<YamlHandleInt>(stringCode::heal_or_shield);
    keyToFunc["damage if full health"] = std::make_shared<YamlHandleInt>(stringCode::damage_if_full_health);
    keyToFunc["kill if below threshold"] = std::make_shared<YamlHandleInt>(stringCode::kill_if_below_threshold);

    keyToFunc["level"] = std::make_shared<YamlHandleInt>(stringCode::item_level);
    keyToFunc["cost"] = std::make_shared<YamlHandleInt>(stringCode::cost);
    keyToFunc["slot"] = std::make_shared<YamlHandleInt>(stringCode::item_slot);


}

void YamlReader::readFile(const std::string &name) {
    /// read file and put all data into a string
    std::string fileName = "../src/io/gamedata/" + name + ".dgr";
    std::ifstream ifile;
    ifile.open(fileName);
    if (!ifile) { // file couldn't be opened
        std::cerr << "[YamlReader::readfile] file could not be opened" << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << ifile.rdbuf();
    ifile.close();

    std::string worldStr = Utilities::trim(buffer.str());
    worldStr = Utilities::checkAndRemoveDGRFileVersion(worldStr, name);
    if (worldStr.empty()) {
        std::cerr << "[YamlReader::readfile] error file reading version" << std::endl;
        exit(-1);
    }

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
