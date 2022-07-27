//
// Created by thijs on 18-06-22.
//

#ifndef DICEGONEROGUE_MAINMENUSCENE_H
#define DICEGONEROGUE_MAINMENUSCENE_H

#include "Scene.h"

namespace DGR {

class MainMenuScene : public Scene {
private:
    void pressButton(const std::unique_ptr<Button> &button) override;

public:
    explicit MainMenuScene(std::weak_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;
};

}


#endif //DICEGONEROGUE_MAINMENUSCENE_H
