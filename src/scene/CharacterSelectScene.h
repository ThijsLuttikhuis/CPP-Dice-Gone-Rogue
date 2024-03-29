//
// Created by thijs on 21-06-22.
//

#ifndef DICEGONEROGUE_CHARACTERSELECTSCENE_H
#define DICEGONEROGUE_CHARACTERSELECTSCENE_H


#include <gameobject/Character.h>
#include "Scene.h"

namespace DGR {

class CharacterSelectScene : public Scene {
private:
    int maxSelect = 4;
    std::vector<std::shared_ptr<Character>> selectedHeroes;

    int maxCharactersOnRow = 4;
    int currentLeftCharacterIndex = 0;

    void alignCharacterPositions();

    void pressButton(const std::unique_ptr<Button> &button) override;
public:
    explicit CharacterSelectScene(std::weak_ptr<GameStateManager> gameState);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void update(double dt) override;

    void handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos);

    bool handleHeroesMouseButton(double xPos, double yPos);

    void render(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                const std::unique_ptr<TextRenderer> &textRenderer) const override;
};

}


#endif //DICEGONEROGUE_CHARACTERSELECTSCENE_H
