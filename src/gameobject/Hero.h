//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_HERO_H
#define DICEGONEROGUE_HERO_H

#include <utility>

#include "GameObject.h"

namespace DGR {

class Dice;

class Hero : public GameObject {
private:
    Dice* dice = nullptr;

    int hp{};
    int maxHP{};

public:
    explicit Hero(std::string name) : GameObject(std::move(name)) {};

    Hero(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    [[nodiscard]] Dice* getDice() const;

    void setDice(Dice* dice);

    void setMaxHP(int i);

    void draw(SpriteRenderer* spriteRenderer) override;

    void drawHover(SpriteRenderer* spriteRenderer);
};

}

#endif //DICEGONEROGUE_HERO_H
