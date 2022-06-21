//
// Created by thijs on 18-06-22.
//

#ifndef DICEGONEROGUE_MAINMENUSCENE_H
#define DICEGONEROGUE_MAINMENUSCENE_H

#include "Scene.h"

namespace DGR {

class MainMenuScene : public Scene {
private:
    void pressButton(Button* button);
public:
    explicit MainMenuScene(GameStateManager* gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;
};

}


#endif //DICEGONEROGUE_MAINMENUSCENE_H
