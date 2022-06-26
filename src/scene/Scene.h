//
// Created by thijs on 17-06-22.
//

#ifndef DICEGONEROGUE_SCENE_H
#define DICEGONEROGUE_SCENE_H


#include <ui/UIElement.h>
#include <utilities/Constants.h>
#include "ui/InputHandler.h"

namespace DGR {

class GameStateManager;

class Scene : public InputHandler, public UIElement {
protected:
    bool enabled = true;
public:
    Scene(std::string name, GameStateManager* gameState);

    Scene(std::string name, GameStateManager* gameState, glm::vec2 position,
          glm::vec2 size = glm::vec2(DGR_WIDTH, DGR_HEIGHT), glm::vec3 color = glm::vec3(1.0f));

    /// getters
    [[nodiscard]] bool isEnabled() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const override;

    [[nodiscard]] GameStateManager* getGameStateManager() const;

    /// setters
    void setIsEnabled(bool enabled_);

    /// functions
    virtual void reset();

    virtual void update(double dt);
};

}


#endif //DICEGONEROGUE_SCENE_H
