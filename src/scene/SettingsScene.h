//
// Created by thijs on 19-06-22.
//

#ifndef DICEGONEROGUE_SETTINGSSCENE_H
#define DICEGONEROGUE_SETTINGSSCENE_H


#include "Scene.h"

namespace DGR {

class SettingsScene : public Scene {

public:
    explicit SettingsScene(std::weak_ptr<GameStateManager> gameState);

    void handleMouseButton(double xPos, double yPos) override;

};

}


#endif //DICEGONEROGUE_SETTINGSSCENE_H
