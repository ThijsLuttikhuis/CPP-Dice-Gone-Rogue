//
// Created by thijs on 17-06-22.
//

#ifndef DICEGONEROGUE_SCENE_H
#define DICEGONEROGUE_SCENE_H


#include <GameStateManager.h>
#include "ui/InputHandler.h"

namespace DGR {

class Scene : public InputHandler {
protected:
    bool enabled = false;
public:
    explicit Scene(GameStateManager* gameState);

    [[nodiscard]] bool isEnabled() const;

    void setIsEnabled(bool enabled_);

    virtual void update(double dt) = 0;
};

}


#endif //DICEGONEROGUE_SCENE_H
