//
// Created by thijs on 20-07-22.
//

#ifndef DICEGONEROGUE_GAMEPROGRESS_H
#define DICEGONEROGUE_GAMEPROGRESS_H

#include <string>
#include <memory>
#include <gameobject/Item.h>
#include <vector>

namespace DGR {
class Inventory;

class GameProgress {
private:
    int unlockedLevel = 1;
    int numberOfLevels = 20;

    std::vector<std::shared_ptr<Item>> itemsToGet;
public:
    GameProgress() = default;

    /// getters
    [[nodiscard]] int getUnlockedLevel() const;

    [[nodiscard]] int getNumberOfLevels() const;

    [[nodiscard]] bool areThereItemsToGet() const;

    /// setters
    void setUnlockedLevel(int level);

    /// functions
    static int levelToEnemyStrength(int selectedLevel);

    static int enemyNameToStrength(const std::string &enemyName);

    void reset();

    void completeLevel(int level, const std::unique_ptr<Inventory> &inventory);
};

}


#endif //DICEGONEROGUE_GAMEPROGRESS_H
