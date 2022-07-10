//
// Created by thijs on 11-06-22.
//

#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>

#include "scene/BattleScene.h"
#include "BattleLog.h"
#include "GameStateManager.h"

namespace DGR {

int BattleLog::heroesTurnCounter = 0;
int BattleLog::enemiesTurnCounter = 0;

BattleLog::BattleLog(std::weak_ptr<BattleScene> battleScene)
      : battleScene(std::move(battleScene)) {}

std::shared_ptr<Character> BattleLog::getStoredCharacter(int id) {
    for (auto &hero : heroes) {
        if (hero->getUniqueID() == id) {
            return hero;
        }
    }
    for (auto &enemy : enemies) {
        if (enemy->getUniqueID() == id) {
            return enemy;
        }
    }

    std::cerr << "AttackOrder::getStoredCharacter: error: no character found!" << std::endl;
    exit(404);
}

std::shared_ptr<Spell> BattleLog::getStoredSpell(int id) {
    for (auto &hero : heroes) {
        if (hero->getUniqueID() == id) {
            return hero->getSpell();
        }
    }
    for (auto &enemy : enemies) {
        if (enemy->getUniqueID() == id) {
            return enemy->getSpell();
        }
    }

    std::cerr << "AttackOrder::getStoredSpell: error: no spell found!" << std::endl;
    exit(404);
}

void BattleLog::addAttack(Character* character, Character* otherCharacter) {
    thisTurn.attackOrder.push_back(attackType::character);

    int characterID = character->getUniqueID();
    int otherCharacterID = otherCharacter ? otherCharacter->getUniqueID() : -1;

    thisTurn.attackOrderIDs.emplace_back(characterID, otherCharacterID);
}

void BattleLog::addAttack(Spell* spell, Character* character) {
    thisTurn.attackOrder.push_back(attackType::spell);

    int characterID = character->getUniqueID();
    int spellID = std::shared_ptr<Character>(spell->getCharacter())->getUniqueID();

    thisTurn.attackOrderIDs.emplace_back(characterID, spellID);
}

void BattleLog::undo() {
    if (thisTurn.attackOrder.empty()) {
        return;
    }
    auto battleScenePtr = std::shared_ptr<BattleScene>(battleScene);
    battleScenePtr->setMana(mana);

    // create new copies
    std::vector<std::shared_ptr<Character>> heroes_ = {};
    std::vector<std::shared_ptr<Character>> enemies_ = {};
    for (auto &hero : heroes) {
        std::shared_ptr<Character> copy = hero->makeCopy(true);
        heroes_.push_back(copy);
    }
    for (auto &enemy : enemies) {
        std::shared_ptr<Character> copy = enemy->makeCopy(true);
        enemies_.push_back(copy);
    }

    battleScenePtr->setHeroes(heroes);
    battleScenePtr->setEnemies(enemies);

    // apply attacks until index-1
    int undoTillIndex = (int) thisTurn.attackOrder.size() - 1;
    for (int i = 0; i < undoTillIndex; i++) {
        if (thisTurn.attackOrder[i] == attackType::character) {
            auto ids = thisTurn.attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto otherCharacter = ids.second >= 0 ? getStoredCharacter(ids.second) : nullptr;
            auto success = character->interact(otherCharacter, battleScenePtr, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(character) unsuccessful!" << std::endl;
                exit(-10);
            }
            if (otherCharacter) {
                otherCharacter->setUsedDice(true);
            } else {
                character->setUsedDice(true);
            }
        } else if (thisTurn.attackOrder[i] == attackType::spell) {
            auto ids = thisTurn.attackOrderIDs[i];
            auto character = getStoredCharacter(ids.first);
            auto spell = ids.second >= 0 ? getStoredSpell(ids.second) : nullptr;
            auto success = character->interact(spell, battleScenePtr, false);
            if (!success) {
                std::cerr << "AttackOrder::undo(): character->interact(spell) unsuccessful!" << std::endl;
                exit(-10);
            }
            battleScenePtr->addMana(-spell->getCost());
        }
    }

    thisTurn.attackOrder.pop_back();
    thisTurn.attackOrderIDs.pop_back();

    std::swap(heroes, heroes_);
    std::swap(enemies, enemies_);
}

std::tuple<std::vector<std::shared_ptr<Character>>*,
      std::vector<std::shared_ptr<Character>>*,
      int> BattleLog::getState() {

    return std::make_tuple(&heroes, &enemies, mana);
}

void BattleLog::setState(const std::vector<std::shared_ptr<Character>> &heroes_,
                         const std::vector<std::shared_ptr<Character>> &enemies_, int mana_) {
    thisTurn.attackOrder = {};
    thisTurn.attackOrderIDs = {};
    mana = mana_;

    heroes = {};
    enemies = {};

    // set new copies
    for (auto &hero : heroes_) {
        std::shared_ptr<Character> copy = hero->makeCopy(true);
        heroes.push_back(copy);
    }
    for (auto &enemy : enemies_) {
        std::shared_ptr<Character> copy = enemy->makeCopy(true);
        enemies.push_back(copy);
    }
}

void BattleLog::reset() {
    setState({}, {}, 0);

    thisTurn.attackOrder = {};
    thisTurn.attackOrderIDs = {};
    turns = {};
}

void BattleLog::saveTurn(bool heroesAttacked) {
    thisTurn.heroesAttacked = heroesAttacked;
    turns.push_back(thisTurn);

    std::string fileString;
    if (heroesTurnCounter == 0 && enemiesTurnCounter == 0) {
        fileString += (std::string) ".dgr.version=" + DGR_FILE_VERSION + "\n\n";
        fileString += idsCharacternamesToString();
    }
    if (thisTurn.heroesAttacked) {
        fileString += "hero turn " + std::to_string(++heroesTurnCounter) + ";\n";
    } else {
        fileString += "enemy turn " + std::to_string(++enemiesTurnCounter) + ";\n";
    }
    fileString += idsFaceidsToString();
    fileString += thisTurn.toString();

    std::ofstream ofile;
    std::string fileName = "../src/io/saves/" + DGR_SAVEGAMENAME + ".save.dgr";
    ofile.open(fileName, std::fstream::app);
    ofile << fileString;
    ofile.flush();
    ofile.close();
}

std::string BattleLog::idsFaceidsToString() const {
    std::string idFaceString;
    for (const auto &hero : heroes) {
        idFaceString += "id: " + std::to_string(hero->getUniqueID()) +
                        ", faceid: " + std::to_string(hero->getDice()->getCurrentFace()->getFace_()) + ";\n";
    }
    for (const auto &enemy : enemies) {
        idFaceString += "id: " + std::to_string(enemy->getUniqueID()) +
                        ", faceid: " + std::to_string(enemy->getDice()->getCurrentFace()->getFace_()) + ";\n";
    }
    idFaceString += "\n";
    return idFaceString;
}

std::string BattleLog::idsCharacternamesToString() const {
    std::string idCharacterString;
    for (const auto &hero : heroes) {
        idCharacterString += "id: " + std::to_string(hero->getUniqueID()) + ", name: " + hero->getName() + ";\n";
    }
    for (const auto &enemy : enemies) {
        idCharacterString += "id: " + std::to_string(enemy->getUniqueID()) + ", name: " + enemy->getName() + ";\n";
    }
    idCharacterString += "\n";
    return idCharacterString;
}

std::shared_ptr<BattleLog> BattleLog::loadBattle(const std::shared_ptr<GameStateManager> &gameState,
                                                 const std::string &fileName) {
    auto battle = std::make_shared<BattleLog>();

    std::ifstream ifile;
    ifile.open(fileName);
    if (!ifile) { // file couldn't be opened
        std::cerr << "[BattleLog::loadBattle] file could not be opened" << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << ifile.rdbuf();
    ifile.close();

    std::string battleString = Utilities::trim(buffer.str());
    battleString = Utilities::checkAndRemoveDGRFileVersion(battleString, fileName);
    if (battleString.empty()) {
        return {};
    }

    battleString = loadBattleHeader(gameState, battleString, battle);

    return battle;
}

std::string BattleLog::loadBattleHeader(const std::shared_ptr<GameStateManager> &gameState, std::string battleString,
                                        std::shared_ptr<BattleLog> &battleLog) {

    std::vector<std::shared_ptr<Character>> heroes;
    std::vector<std::shared_ptr<Character>> enemies;

    battleString = Utilities::trim(battleString);

    std::string line;
    std::string word;
    int i = 0;

    size_t posSemiColon = 0;
    size_t posComma;

    while (i < (int) battleString.length()) {
        posSemiColon = battleString.find(';', i);
        line = Utilities::trim(battleString.substr(i, posSemiColon - i));
        std::cout << line << std::endl;

        /// get id
        if (!(line.substr(0, 3) == "id:")) {
            std::cout << ":(" << std::endl;
            break;
        }
        posComma = line.find(',', 0);
        word = Utilities::trim(line.substr(3, posComma));
        char** ptr = nullptr;
        int id = (int) strtol(word.c_str(), ptr, 10);

        /// get name
        word = Utilities::trim(line.substr(posComma + 1, posSemiColon));
        if (!(word.substr(0, 5) == "name:")) {
            std::cout << ":(2" << std::endl;
            break;
        }
        std::string name = Utilities::trim(word.substr(5, word.length() - 5));

        /// get character
        std::cout << id << ", " << name << std::endl;
        auto character = gameState->getCharacterByID(id);
        if (!character.get()) {
            break;
        }

        if (character->getCharacterType() == "hero") {
            heroes.push_back(character);
        }

        i = (int) posSemiColon + 1;


    }
    battleLog->setState(heroes, enemies, 0);
    return battleString.substr(posSemiColon + 1, battleString.length() - (posSemiColon + 1));
}

std::string BattleLog::Turn::toString() const {
    std::string turnString;
    for (int i = 0; i < (int) attackOrder.size(); i++) {
        turnString += std::string(attackOrder[i] == attackType::character ?
                                  "type: character, dst: " : "type: spell, dst: ") +=
              std::to_string(attackOrderIDs[i].first) + ", src: " +
              std::to_string(attackOrderIDs[i].second) + ";\n";
    }
    turnString += "\n";
    return turnString;
}

BattleLog::Turn BattleLog::Turn::getTurnFromString(std::string turnString) {
    //TODO:
    Turn turn;
    std::pair<int, int> ids;
    int idNotSet = -12345;
    turnString = Utilities::trim(turnString);
    std::string word;
    size_t i = 0;
    auto turnStrLength = turnString.size();

    while (i < turnStrLength) {
        size_t posComma = turnString.find(',', i);
        size_t posSemiColon = turnString.find(',', i);

        word = Utilities::trim(turnString.substr(i, posComma - i));
        i = posComma + 1;

        if (word == "character") {
            turn.attackOrder.push_back(attackType::character);
        } else if (word == "spell") {
            turn.attackOrder.push_back(attackType::spell);
        } else if (word.substr(0, 4) == "dst:") {
            char** ptr = nullptr;
            ids.first = (int) strtol(word.c_str() + 4, ptr, 10);
        } else if (word.substr(0, 4) == "src:") {
            char** ptr = nullptr;
            ids.second = (int) strtol(word.c_str() + 4, ptr, 10);
        }

        if (ids.first != idNotSet && ids.second != idNotSet) {
            turn.attackOrderIDs.push_back(ids);
            ids.first = idNotSet;
            ids.second = idNotSet;
        }

    }
    return turn;
}

}
