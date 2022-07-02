//
// Created by thijs on 19-06-22.
//

#ifndef DICEGONEROGUE_SETTINGSSCENE_H
#define DICEGONEROGUE_SETTINGSSCENE_H


#include "Scene.h"

namespace DGR {

class SettingsScene : public Scene {
private:
    void pressButton( std::shared_ptr<Button> button);
public:
    explicit SettingsScene( std::shared_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) override;
};

}


#endif //DICEGONEROGUE_SETTINGSSCENE_H
