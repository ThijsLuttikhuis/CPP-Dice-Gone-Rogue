//
// Created by thijs on 12-07-22.
//

#ifndef DICEGONEROGUE_AREYOUSURESCENE_H
#define DICEGONEROGUE_AREYOUSURESCENE_H


#include "Scene.h"

namespace DGR {

class AreYouSureScene : public Scene {
private:
    void pressButton(std::shared_ptr<Button> button);
    std::string messageSource;
    std::string messageData;

public:
    explicit AreYouSureScene(std::weak_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) override;

    std::string message(const std::string &data) override;
};

}


#endif //DICEGONEROGUE_AREYOUSURESCENE_H
