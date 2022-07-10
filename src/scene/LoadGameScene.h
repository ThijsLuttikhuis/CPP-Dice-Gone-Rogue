//
// Created by thijs on 10-07-22.
//

#ifndef DICEGONEROGUE_LOADGAMESCENE_H
#define DICEGONEROGUE_LOADGAMESCENE_H


#include <gameobject/Character.h>
#include <utilities/BattleLog.h>
#include "Scene.h"

namespace DGR {

class LoadGameScene : public Scene {
private:
    std::vector<std::shared_ptr<BattleLog>> loadedGames;
    int selectedGame = 0;

    int maxSavesOnRow = 4;
    int currentLeftSaveIndex = 0;

    void alignSavesPositions();

public:
    explicit LoadGameScene(std::weak_ptr<GameStateManager> gameState);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void reset() override;

    void update(double dt) override;

    void handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos);

    void pressButton(std::shared_ptr<Button> button);

    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) override;

    void initialize() override;

};

}


#endif //DICEGONEROGUE_LOADGAMESCENE_H
