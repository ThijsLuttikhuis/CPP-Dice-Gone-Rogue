//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_CHARACTER_H
#define DICEGONEROGUE_CHARACTER_H

#include <gameobject/dice/Dice.h>
#include "GameObject.h"

namespace DGR {

class Character : public GameObject {
private:
    Dice* dice = nullptr;

    int hp{};
    int maxHP{};
public:
    explicit Character(std::string name) : GameObject(std::move(name)) {};

    Character(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    [[nodiscard]] Dice* getDice() const;

    void setDice(Dice* dice);

    void setMaxHP(int i);

    void draw(SpriteRenderer* spriteRenderer) override;

    void drawHover(SpriteRenderer* spriteRenderer);
};

}

#endif //DICEGONEROGUE_CHARACTER_H
