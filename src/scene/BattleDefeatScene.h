//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEDEFEATSCENE_H
#define DICEGONEROGUE_BATTLEDEFEATSCENE_H


#include "Scene.h"

namespace DGR {

class BattleDefeatScene : public Scene {
private:
    void pressButton(const std::shared_ptr<Button> &button);

public:
    explicit BattleDefeatScene(std::weak_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) const override;

    void onPopFromStack();
};

}


#endif //DICEGONEROGUE_BATTLEDEFEATSCENE_H
