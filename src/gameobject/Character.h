//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_CHARACTER_H
#define DICEGONEROGUE_CHARACTER_H

#include <memory>
#include <utility>

#include "dice/Dice.h"
#include "GameObject.h"

namespace DGR {

class Face;

class Spell;

class GameStateManager;

class BattleScene;

class Character : public GameObject {
private:
    std::string characterType;

     std::shared_ptr<Dice> dice = nullptr;
     std::shared_ptr<Spell> spell = nullptr;

    int hp = 0;
    int maxHP = 0;
    int shield = 0;
    int incomingDamage = 0;
    int incomingPoison = 0;
    int incomingRegen = 0;
    int poison = 0;
    int regen = 0;
    bool isDodging = false;
    bool isUndying = false;
    bool backRow = false;

    void applySpellTypeDamage( std::shared_ptr<Spell> spell,  std::shared_ptr<BattleScene> battleScene);

    void applyFaceTypeDamage( std::shared_ptr<Face> face,  std::shared_ptr<BattleScene> battleScene);

    void applyFaceTypeHeal( std::shared_ptr<Face> face,  std::shared_ptr<BattleScene> battleScene);

    void applyFaceTypeShield( std::shared_ptr<Face> face,  std::shared_ptr<BattleScene> battleScene);

    void applyFaceTypeBonusHealth( std::shared_ptr<Face> face,  std::shared_ptr<BattleScene> battleScene);

    void applyFaceModifierCleanse( std::shared_ptr<Face> face,  std::shared_ptr<BattleScene> battleScene);

    void applyFaceModifierSweepingEdge(FaceType::faceType type,  std::shared_ptr<Face> face,  std::shared_ptr<BattleScene> battleScene);

    void drawHealthBar( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) const;

public:
    Character(std::string name, std::string characterType);

    Character(const std::string &name, std::string characterType, glm::vec2 position, glm::vec2 size);

    /// getters
    [[nodiscard]]  std::shared_ptr<Dice> getDice() const;

    [[nodiscard]] bool isDead() const;

    [[nodiscard]] bool getUsedDice() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, bool alsoCurrentFacehover = false) const;

    [[nodiscard]] int getIncomingDamage() const;

    [[nodiscard]] std::string getCharacterType() const;

    [[nodiscard]]  std::shared_ptr<Spell> getSpell() const;

    [[nodiscard]]  std::shared_ptr<Character> makeCopy(bool copyUniqueID = false) const;

    /// setters
    void setDice( std::shared_ptr<Dice> dice);

    void setDiceLock(bool diceLock_);

    void setHP(int hp);

    void setMaxHP(int maxHP_, bool setHPToMaxHP = true);

    void setIncomingDamage(int incomingDamage_);

    void setIncomingRegen(int incomingRegen_);

    void setPoison(int poison_);

    void setBackRow(bool backRow_);

    void setRegen(int regen_);

    void setUsedDice(bool usedDice);

    void setSpell( std::shared_ptr<Spell> spell_);

    void setDodging(bool isDodging_);

    void setUndying(bool isUndying_);

    void setShield(int shield_);

    void setIncomingPoison(int incomingPoison_);

    /// functions
    void roll();

    void toggleDiceLock();

    bool interact( std::shared_ptr<Character> otherCharacter,  std::shared_ptr<BattleScene> battleScene, bool storeAction = true);

    bool interact( std::shared_ptr<Spell> clickedSpell,  std::shared_ptr<BattleScene> battleScene, bool storeAction = true);

    void applyDamageStep();

    /// render
    void draw( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) const override;

    void drawBox( std::shared_ptr<SpriteRenderer> spriteRenderer, glm::vec3 color) const;

    void drawHover( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) const;

    void applySpellTypeCleanse( std::shared_ptr<Spell> spell,  std::shared_ptr<BattleScene> battleScene);
};

}

#endif //DICEGONEROGUE_CHARACTER_H
