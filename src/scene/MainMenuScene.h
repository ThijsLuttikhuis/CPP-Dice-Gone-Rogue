//
// Created by thijs on 18-06-22.
//

#ifndef DICEGONEROGUE_MAINMENUSCENE_H
#define DICEGONEROGUE_MAINMENUSCENE_H

#include "Scene.h"

namespace DGR {

class MainMenuScene : public Scene {
private:
    void pressButton( std::shared_ptr<Button> button);
public:
    explicit MainMenuScene( std::shared_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) override;
};

}


#endif //DICEGONEROGUE_MAINMENUSCENE_H
