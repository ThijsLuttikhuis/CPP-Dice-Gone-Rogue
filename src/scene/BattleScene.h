//
// Created by thijs on 17-06-22.
//

#ifndef DICEGONEROGUE_BATTLESCENE_H
#define DICEGONEROGUE_BATTLESCENE_H


#include <vector>
#include <gamelogic/BattleController.h>

#include "Scene.h"
#include "gamelogic/BattleLog.h"
#include "ui/Window.h"
#include "gameobject/spell/Spell.h"

namespace DGR {

class BattleScene : public std::enable_shared_from_this<BattleScene>, public Scene {
public:

private:
    void alignCharacterPositions(double dt);

    void updateCharacterKeyPresses();

    void pressButton(const std::unique_ptr<Button> &button) override;

    void handleCharacterMousePosition(std::shared_ptr<Character> character, double xPos, double yPos);

    void updateButtons();

public:
    explicit BattleScene(std::weak_ptr<GameStateManager> gameState);

    [[nodiscard]] std::shared_ptr<BattleScene> getSharedFromThis();

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void handleKeyboard(int key, int action, const std::unique_ptr<std::vector<bool>> &keysPressed) override;

    void update(double dt) override;

    std::string message(const std::string &data) override;

    /// render
    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;

};

}


#endif //DICEGONEROGUE_BATTLESCENE_H
