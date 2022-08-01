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
    int maxSelect = 2;
    std::vector<std::shared_ptr<Character>> selectedHeroes;

    int maxCharactersOnRow = 4;
    int currentLeftCharacterIndex = 0;

    void alignCharacterPositions();

public:
    explicit CharacterSelectScene(std::weak_ptr<GameStateManager> gameState);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void update(double dt) override;

    void handleMousePosition(std::shared_ptr<Character> character, double xPos, double yPos);

    void pressButton(std::shared_ptr<Button> button);

    bool handleHeroesMouseButton(double xPos, double yPos);

    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) override;
};

}


#endif //DICEGONEROGUE_CHARACTERSELECTSCENE_H
