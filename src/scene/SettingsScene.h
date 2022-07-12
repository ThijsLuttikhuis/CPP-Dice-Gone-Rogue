//
// Created by thijs on 19-06-22.
//

#ifndef DICEGONEROGUE_SETTINGSSCENE_H
#define DICEGONEROGUE_SETTINGSSCENE_H


#include "Scene.h"

namespace DGR {

class SettingsScene : public Scene {
private:
    void pressButton(Button* button);
public:
    explicit SettingsScene(GameStateManager* gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;
};

}


#endif //DICEGONEROGUE_SETTINGSSCENE_H
