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

    int hp = 0;
    int maxHP = 0;
    int shield = 0;
    int poison = 0;
    int regen = 0;
public:
    explicit Character(std::string name) : GameObject(std::move(name)) {};

    Character(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    [[nodiscard]] Dice* getDice() const;

    [[nodiscard]] bool isDead() const;

    [[nodiscard]] bool getUsedDice() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, bool alsoCurrentFacehover = false) const;

    void setDice(Dice* dice);

    void setDiceLock(bool diceLock_);

    void setMaxHP(int maxHP_, bool setHPToMaxHP = true);

    bool interact(Character* otherCharacter);

    void roll();

    void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;

    void drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    virtual std::string getCharacterType() = 0;

    void toggleDiceLock();

    void drawBox(SpriteRenderer* spriteRenderer, glm::vec3 color);


    void setUsedDice(bool usedDice);

};

}

#endif //DICEGONEROGUE_CHARACTER_H
