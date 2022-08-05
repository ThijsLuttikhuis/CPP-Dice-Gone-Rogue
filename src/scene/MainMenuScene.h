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
};

}


#endif //DICEGONEROGUE_MAINMENUSCENE_H
