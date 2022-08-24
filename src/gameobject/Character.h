//
// Created by thijs on 04-06-22.
//

#ifndef DICEGONEROGUE_CHARACTER_H
#define DICEGONEROGUE_CHARACTER_H

#include <memory>
#include <utility>
#include <ui/Button.h>

#include "dice/Dice.h"
#include "GameObject.h"
#include "ui/UIElement.h"

namespace DGR {

class Face;

class Spell;

class GameStateManager;

class BattleScene;

class Character : public std::enable_shared_from_this<Character>, public Button {
public:
    enum hoverType {
        onlyHero,
        onlyCurrentFace,
        heroAndCurrentFace,
        extendedBox
    };
private:
    std::string characterType;

    std::shared_ptr<Dice> dice = nullptr;
    std::shared_ptr<Spell> spell = nullptr;

    /// xp stats
    int characterLevel = 1;
    int xp = 0;

    /// battle stats
    int level1DefaultMaxHP = 0;
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

    /// level bonus stats
    double hpFactorPerLevel = 1.2;
    int getMaxHpAtLevel(int level);

    void applySpellTypeDamage(const std::shared_ptr<Spell> &spell, const std::shared_ptr<BattleScene> &battleScene);

    void applySpellTypeCleanse(const std::shared_ptr<Spell> &spell, const std::shared_ptr<BattleScene> &battleScene);

    void applyFaceTypeDamage(const std::shared_ptr<Face> &face, const std::shared_ptr<BattleScene> &battleScene);

    void applyFaceTypeHeal(const std::shared_ptr<Face> &face, const std::shared_ptr<BattleScene> &battleScene);

    void applyFaceTypeShield(const std::shared_ptr<Face> &face, const std::shared_ptr<BattleScene> &battleScene);

    void applyFaceTypeBonusHealth(const std::shared_ptr<Face> &face, const std::shared_ptr<BattleScene> &battleScene);

    void applyFaceModifierCleanse(const std::shared_ptr<Face> &face, const std::shared_ptr<BattleScene> &battleScene);

    void applyFaceModifierSweepingEdge(FaceType::faceType type, const std::shared_ptr<Face> &face,
                                       const std::shared_ptr<BattleScene> &battleScene);

    void drawHealthBar(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                       const std::unique_ptr<TextRenderer> &textRenderer) const;

public:
    Character(std::string name, std::string characterType);

    /// getters
    [[nodiscard]] std::shared_ptr<Character> getSharedFromThis();

    [[nodiscard]]  std::shared_ptr<Dice> getDice() const;

    [[nodiscard]] bool isDead() const;

    [[nodiscard]] bool getUsedDice() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, hoverType hoverType = onlyHero) const;

    [[nodiscard]] int getIncomingDamage() const;

    [[nodiscard]] std::string getCharacterType() const;

    [[nodiscard]] std::shared_ptr<Spell> getSpell() const;

    [[nodiscard]] std::shared_ptr<Character> makeCopy(bool copyUniqueID = false) const;

    [[nodiscard]] bool canLevelUp() const;

    [[nodiscard]] int getLevel() const;

    [[nodiscard]] double getXPBarFill(double percent) const;

    [[nodiscard]] int xpToLevelUp(int currentLevel) const;

    /// setters
    void setDice(const std::shared_ptr<Dice> &dice);

    void setDiceLock(bool diceLock_);

    void setHP(int hp);

    void setXP(int xp);

    void setLevel(int level);

    void setMaxHP(int maxHP_, bool setHPToMaxHP = true);

    void setIncomingDamage(int incomingDamage_);

    void setIncomingRegen(int incomingRegen_);

    void setPoison(int poison_);

    void setBackRow(bool backRow_);

    void setRegen(int regen_);

    void setUsedDice(bool usedDice);

    void setSpell(const std::shared_ptr<Spell> &spell_);

    void setDodging(bool isDodging_);

    void setUndying(bool isUndying_);

    void setShield(int shield_);

    void setIncomingPoison(int incomingPoison_);

    void addXP(int xp_);

    /// functions
    void roll();

    void levelUp();

    void toggleDiceLock();

    bool interact(const std::shared_ptr<Character> &otherCharacter,
                  const std::shared_ptr<BattleScene> &battleScene,
                  bool storeAction = true);

    bool interact(const std::shared_ptr<Spell> &clickedSpell,
                  const std::shared_ptr<BattleScene> &battleScene,
                  bool storeAction = true);

    void applyDamageStep();

    /// render
    void draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
              const std::unique_ptr<TextRenderer> &textRenderer) const override;

    void drawBox(const std::unique_ptr<SpriteRenderer> &spriteRenderer, glm::vec3 color) const;

    void drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                   const std::unique_ptr<TextRenderer> &textRenderer, bool drawCurrentFaceIndicator = false) const;

    void drawShadow(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                    const std::unique_ptr<TextRenderer> &textRenderer) const;

    void drawXPBar(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                   const std::unique_ptr<TextRenderer> &textRenderer, double xpPercent);

    void drawHeroOnly(const std::unique_ptr<SpriteRenderer> &spriteRenderer) const;

    void drawLevelUp(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer);

    void drawKey(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                 const std::unique_ptr<TextRenderer> &textRenderer) const override;
};

}

#endif //DICEGONEROGUE_CHARACTER_H
