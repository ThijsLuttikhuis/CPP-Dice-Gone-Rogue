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
    double t = 0.0;
public:
    Scene(std::string name, std::weak_ptr<GameStateManager> gameState);

    Scene(std::string name, std::weak_ptr<GameStateManager> gameState, glm::vec2 position,
          glm::vec2 size = glm::vec2(DGR_WIDTH, DGR_HEIGHT), glm::vec3 color = glm::vec3(1.0f));

    /// getters
    [[nodiscard]] bool isEnabled() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const override;

    [[nodiscard]]  std::weak_ptr<GameStateManager> getGameStateManager() const;

    /// setters
    void setIsEnabled(bool enabled_);

    /// functions
    virtual void initialize();

    virtual void onPushToStack();

    virtual void onPopFromStack();

    virtual void reset();

    virtual std::string message(const std::string &data);

    virtual void update(double dt);

    void handleKeyboard(int key, int action, const std::unique_ptr<std::vector<bool>> &keysPressed) override;

    void handleMouseButton(double xPos, double yPos) override;

    void handleKeyboardDefault(int key, int action, const std::unique_ptr<std::vector<bool>> &keysPressed);

    void handleMouseButtonDefault(double xPos, double yPos);

    void pressButton(const std::unique_ptr<Button> &button) override;

    bool pressDefaultButton(const std::unique_ptr<Button> &button);

    static std::unique_ptr<Button> makeDefaultButton(const std::string &string, glm::vec2 position, glm::vec2 size);

    void updateDefaults(double dt);

    /// render
    void renderDefaults(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                        const std::unique_ptr<TextRenderer> &textRenderer) const;

    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;
};

}


#endif //DICEGONEROGUE_SCENE_H
