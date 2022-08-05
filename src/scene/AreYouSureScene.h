//
// Created by thijs on 12-07-22.
//

#ifndef DICEGONEROGUE_AREYOUSURESCENE_H
#define DICEGONEROGUE_AREYOUSURESCENE_H


#include "Scene.h"

namespace DGR {

class AreYouSureScene : public Scene {
private:
    std::string messageSource;
    std::string messageData;

    void pressButton(const std::unique_ptr<Button> &button) override;

public:
    explicit AreYouSureScene(std::weak_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

    std::string message(const std::string &data) override;
};

}


#endif //DICEGONEROGUE_AREYOUSURESCENE_H
