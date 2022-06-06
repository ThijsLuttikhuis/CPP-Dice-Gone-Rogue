//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_CHARACTER_H
#define DICEGONEROGUE_CHARACTER_H

#include <gameobject/dice/Dice.h>
#include "GameObject.h"

namespace DGR {

class Face;

class Character : public GameObject {
private:
    Dice* dice = nullptr;

    int hp{};
    int maxHP{};

    int poison;
    int regen;
public:
    explicit Character(std::string name) : GameObject(std::move(name)) {};

    Character(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    [[nodiscard]] Dice* getDice() const;

    [[nodiscard]] bool isDead() const;

    void setDice(Dice* dice);

    void setMaxHP(int maxHP_, bool setHPToMaxHP = true);

    void dealDamage(Face* face);

    void roll();

    void draw(SpriteRenderer* spriteRenderer) override;

    void drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);
};

}

#endif //DICEGONEROGUE_CHARACTER_H
