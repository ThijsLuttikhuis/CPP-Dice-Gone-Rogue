//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEVICTORYSCENE_H
#define DICEGONEROGUE_BATTLEVICTORYSCENE_H


#include "Scene.h"

namespace DGR {

class BattleVictoryScene : public Scene {
private:
    void pressButton(Button* button);
public:
    explicit BattleVictoryScene(GameStateManager* gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;
};

}


#endif //DICEGONEROGUE_BATTLEVICTORYSCENE_H
