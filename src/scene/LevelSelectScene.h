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

    void pressButton(const std::unique_ptr<Button> &button) override;

public:
    explicit LevelSelectScene(std::weak_ptr<GameStateManager> gameState);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void onPushToStack() override;

    void update(double dt) override;


    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;
};

}


#endif //DICEGONEROGUE_LEVELSELECTSCENE_H
