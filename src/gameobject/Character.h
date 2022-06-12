//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_CHARACTER_H
#define DICEGONEROGUE_CHARACTER_H

#include "dice/Dice.h"
#include "GameObject.h"

namespace DGR {

class Face;

class Spell;

class GameStateManager;

class Character : public GameObject {
protected:
    Dice* dice = nullptr;
    Spell* spell = nullptr;

    int hp = 0;
    int maxHP = 0;
    int shield = 0;
    int incomingDamage = 0;
    int poison = 0;
    int regen = 0;
    bool isDodging = false;
    bool isUndying = false;
    bool backRow = false;

    void applySpellTypeDamage(Spell* spell, GameStateManager* gameState);

    void applyFaceTypeDamage(Face* face, GameStateManager* gameState);

    void applyFaceTypeHeal(Face* face, GameStateManager* gameState);

    void applyFaceTypeShield(Face* face, GameStateManager* gameState);

    void applyFaceModifierCleanse(Face* face, GameStateManager* gameState);

    void applyFaceModifierSweepingEdge(FaceType::faceType type, Face* face, GameStateManager* gameState);

    void drawHealthBar(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    void setCopyParameters(Character* copy) const;

public:
    explicit Character(std::string name) : GameObject(std::move(name)) {};

    Character(const std::string &name, glm::vec2 position, glm::vec2 size)
          : GameObject(name, position, size), dice(new Dice(name, this)) {};

    /// getters
    [[nodiscard]] Dice* getDice() const;

    [[nodiscard]] bool isDead() const;

    [[nodiscard]] bool getUsedDice() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, bool alsoCurrentFacehover = false) const;

    [[nodiscard]] int getIncomingDamage() const;

    [[nodiscard]] virtual std::string getCharacterType() const;

    [[nodiscard]] Spell* getSpell() const;

    /// setters
    void setDice(Dice* dice);

    void setDiceLock(bool diceLock_);

    void setHP(int hp);

    void setMaxHP(int maxHP_, bool setHPToMaxHP = true);

    void setIncomingDamage(int incomingDamage_);

    void setPoison(int poison_);

    void setBackRow(bool backRow_);

    void setRegen(int regen_);

    void setUsedDice(bool usedDice);

    void setSpell(Spell* spell_);

    void setDodging(bool isDodging_);

    void setUndying(bool isUndying_);

    void setShield(int shield_);

    /// functions
    void roll();

    void toggleDiceLock();

    bool interact(Character* otherCharacter, GameStateManager* gameState, bool storeAction = true);

    bool interact(Spell* clickedSpell, GameStateManager* gameState, bool storeAction = true);

    void applyDamageStep();

    /// render
    void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;

    void drawBox(SpriteRenderer* spriteRenderer, glm::vec3 color);

    void drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);


};

}

#endif //DICEGONEROGUE_CHARACTER_H
