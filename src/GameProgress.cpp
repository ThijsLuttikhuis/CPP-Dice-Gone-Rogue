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
    //levelToEnemyStrength = {1,2,3,5,7,10,13,16,20,26,32,40,52,64,80};
    return selectedLevel < 5 ? std::vector<int>{1, 2, 3, 5, 7}[selectedLevel] :
           std::vector<int>{10, 13, 16}[(selectedLevel - 5) % 3] * (int) std::pow(2, (int) ((selectedLevel - 5) / 3));

}

int GameProgress::enemyNameToStrength(const std::string &enemyName) {
    static std::map<std::string, int> enemyNameToStrengthMap = {{"dragon",   50},
                                                                {"slime",    8},
                                                                {"skeleton", 2},
                                                                {"rat",      1}};

    return enemyNameToStrengthMap.find(enemyName) != enemyNameToStrengthMap.end() ? enemyNameToStrengthMap[enemyName]
                                                                                  : 99999;
}

void GameProgress::completeLevel(int level, const std::shared_ptr<Inventory> &inventory) {
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