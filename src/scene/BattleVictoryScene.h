//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEVICTORYSCENE_H
#define DICEGONEROGUE_BATTLEVICTORYSCENE_H


#include "Scene.h"

namespace DGR {

class BattleVictoryScene : public Scene {
public:
    enum victoryGameState {
        get_item,
        get_xp,
        get_xp_done,
        level_up_select
    };
private:
    victoryGameState state = get_item;
    double xpPercent = 0;
    int heroToLevelUp = -1;

    void pressButton(const std::unique_ptr<Button> &button) override;

public:
    explicit BattleVictoryScene(std::weak_ptr<GameStateManager> gameState);

    void handleMousePosition(double xPos, double yPos) override;

    void handleMouseButton(double xPos, double yPos) override;

    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;

    void onPushToStack() override;

    void update(double dt) override;

    void onPopFromStack() override;
};

}


#endif //DICEGONEROGUE_BATTLEVICTORYSCENE_H
