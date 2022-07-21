//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEVICTORYSCENE_H
#define DICEGONEROGUE_BATTLEVICTORYSCENE_H


#include "Scene.h"

namespace DGR {

class BattleVictoryScene : public Scene {
private:
    void pressButton(std::shared_ptr<Button> button);

public:
    explicit BattleVictoryScene(std::weak_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) override;
};

}


#endif //DICEGONEROGUE_BATTLEVICTORYSCENE_H
