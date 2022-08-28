//
// Created by thijs on 20-07-22.
//

#include "GameProgress.h"
#include <vector>
#include <cmath>
#include <map>
#include "Inventory.h"

namespace DGR {

int GameProgress::getUnlockedLevel() const {
    return unlockedLevel;
}

int GameProgress::getNumberOfLevels() const {
    return numberOfLevels;
}

void GameProgress::setUnlockedLevel(int level) {
    unlockedLevel = level;
}

int GameProgress::levelToEnemyStrength(int selectedLevel) {
    //levelToEnemyStrength = {1,2,4,6,8,11,14,17,21,25,29,34,39,44,50,56,62};

    int levelStart = 5;
    int numSum = 3;

    selectedLevel += levelStart;
    int divNum = (selectedLevel / numSum);
    int modNum = selectedLevel % numSum;

    int sum = divNum * (divNum + 1) / 2;

    return sum * numSum + modNum * divNum - divNum * numSum;
}

int GameProgress::enemyNameToStrength(const std::string &enemyName) {
    static std::map<std::string, int> enemyNameToStrengthMap = {{"dragon",   50},
                                                                {"slime",    8},
                                                                {"skeleton", 2},
                                                                {"rat",      1}};

    return enemyNameToStrengthMap.find(enemyName) != enemyNameToStrengthMap.end() ? enemyNameToStrengthMap[enemyName]
                                                                                  : 99999;
}

void GameProgress::completeLevel(int level, const std::unique_ptr<Inventory> &inventory) {
    if (level != unlockedLevel) {
        return;
    }
    unlockedLevel++;

    int xpFactor = 50;
    int xpGained = xpFactor * levelToEnemyStrength(level);

    auto &heroes = inventory->getHeroes();
    for (auto &hero : heroes) {
        hero->addXP(xpGained);
    }
}

void GameProgress::reset() {
    unlockedLevel = 1;
}

bool GameProgress::areThereItemsToGet() const {
    return !itemsToGet.empty();
}


}