//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_CHARACTER_H
#define DICEGONEROGUE_CHARACTER_H

#include "dice/Dice.h"
#include "GameObject.h"

namespace DGR {

class Face;

class GameStateManager;

class Character : public GameObject {
private:
    Dice* dice = nullptr;

    int hp = 0;
    int maxHP = 0;
    int shield = 0;
    int incomingDamage = 0;
    int poison = 0;
    int regen = 0;
    bool isDodging = false;
    bool isUndying = false;
    bool backRow = false;

    void applyFaceTypeDamage(Face* face, GameStateManager* gameState);

    void applyFaceTypeHeal(Face* face, GameStateManager* gameState);

    void applyFaceTypeShield(Face* face, GameStateManager* gameState);

    void applyFaceModifierCleanse(Face* face, GameStateManager* gameState);

    void applyFaceModifierSweepingEdge(FaceType::faceType type, Face* face, GameStateManager* gameState);

    void drawHealthBar(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

public:
    explicit Character(std::string name) : GameObject(std::move(name)) {};

    Character(const std::string &textureName, glm::vec2 position, glm::vec2 size);

    /// getters
    [[nodiscard]] Dice* getDice() const;

    [[nodiscard]] bool isDead() const;

    [[nodiscard]] bool getUsedDice() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, bool alsoCurrentFacehover = false) const;

    [[nodiscard]] int getIncomingDamage() const;

    [[nodiscard]] Character* makeUndamagedCopy() const;

    [[nodiscard]] virtual std::string getCharacterType() const;

    /// setters
    void setDice(Dice* dice);

    void setDiceLock(bool diceLock_);

    void setMaxHP(int maxHP_, bool setHPToMaxHP = true);

    void setUsedDice(bool usedDice);

    /// functions
    void roll();

    void toggleDiceLock();

    bool interact(Character* otherCharacter, GameStateManager* gameState);

    void applyDamageStep();

    /// render
    void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;

    void drawBox(SpriteRenderer* spriteRenderer, glm::vec3 color);

    void drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);
};

}

#endif //DICEGONEROGUE_CHARACTER_H
