//
// Created by thijs on 20-07-22.
//

#ifndef DICEGONEROGUE_GAMEPROGRESS_H
#define DICEGONEROGUE_GAMEPROGRESS_H

#include <string>
#include <memory>

namespace DGR {
class Inventory;

class GameProgress {
private:
    int unlockedLevel = 1;
    int numberOfLevels = 20;

public:
    GameProgress() = default;

    /// getters
    [[nodiscard]] int getUnlockedLevel() const;

    [[nodiscard]] int getNumberOfLevels() const;

    /// setters
    void setUnlockedLevel(int level);

    /// functions
    static int levelToEnemyStrength(int selectedLevel);

    static int enemyNameToStrength(const std::string &enemyName);

    void completeLevel(int level, const std::shared_ptr<Inventory> &inventory);
};

}


#endif //DICEGONEROGUE_GAMEPROGRESS_H
