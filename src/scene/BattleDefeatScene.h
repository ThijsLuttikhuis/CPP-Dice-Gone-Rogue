//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEDEFEATSCENE_H
#define DICEGONEROGUE_BATTLEDEFEATSCENE_H


#include "Scene.h"

namespace DGR {

class BattleDefeatScene : public Scene {
private:
    void pressButton(const std::unique_ptr<Button> &button) override;

public:
    explicit BattleDefeatScene(std::weak_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;

    void onPopFromStack() override;
};

}


#endif //DICEGONEROGUE_BATTLEDEFEATSCENE_H
