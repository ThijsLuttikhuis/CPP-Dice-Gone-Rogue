//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEDEFEATSCENE_H
#define DICEGONEROGUE_BATTLEDEFEATSCENE_H


#include "Scene.h"

namespace DGR {

class BattleDefeatScene : public Scene {
private:
    void pressButton(Button* button);
public:
    explicit BattleDefeatScene(GameStateManager* gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;
};

}


#endif //DICEGONEROGUE_BATTLEDEFEATSCENE_H
