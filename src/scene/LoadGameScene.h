//
// Created by thijs on 10-07-22.
//

#ifndef DICEGONEROGUE_LOADGAMESCENE_H
#define DICEGONEROGUE_LOADGAMESCENE_H


#include <gameobject/Character.h>
#include <io/BattleLog.h>
#include "Scene.h"

namespace DGR {

class LoadGameScene : public Scene {
private:
    std::map<std::string, std::shared_ptr<BattleLog>> loadedGames;
    std::vector<std::string> loadedGameNames;

    int selectedGame = 0;

    int maxSavesOnRow = 4;
    int currentLeftSaveIndex = 0;

    void pressButton(const std::unique_ptr<Button> &button) override;

public:
    explicit LoadGameScene(std::weak_ptr<GameStateManager> gameState);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void reset() override;

    void update(double dt) override;

    void handleMousePosition(const std::shared_ptr<Character>& character, double xPos, double yPos);


    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;

    void onPushToStack() override;

    void drawLoadedGame(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                        const std::unique_ptr<TextRenderer> &textRenderer, int index,
                        glm::vec2 position, glm::vec2 size) const;

    std::string message(const std::string &data) override;
};

}


#endif //DICEGONEROGUE_LOADGAMESCENE_H
