//
// Created by thijs on 19-07-22.
//

#ifndef DICEGONEROGUE_LEVELSELECTSCENE_H
#define DICEGONEROGUE_LEVELSELECTSCENE_H


#include "Scene.h"

namespace DGR {

class LevelSelectScene : public Scene {
private:
    int selectedLevel = 0;
    int levelsPerRow = 4;
    int levelsPerPage = 20;

public:
    explicit LevelSelectScene(std::weak_ptr<GameStateManager> gameState);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void update(double dt) override;

    void pressButton(std::shared_ptr<Button> button);

    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) override;
};

}


#endif //DICEGONEROGUE_LEVELSELECTSCENE_H