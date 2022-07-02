//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEDEFEATSCENE_H
#define DICEGONEROGUE_BATTLEDEFEATSCENE_H


#include "Scene.h"

namespace DGR {

class BattleDefeatScene : public Scene {
private:
    void pressButton( std::shared_ptr<Button> button);
public:
    explicit BattleDefeatScene( std::shared_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) override;
};

}


#endif //DICEGONEROGUE_BATTLEDEFEATSCENE_H