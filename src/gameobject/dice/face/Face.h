//
// Created by thijs on 01-06-22.
//

#ifndef DICEGONEROGUE_FACE_H
#define DICEGONEROGUE_FACE_H

#include <memory>
#include <sstream>
#include <string>
#include <map>
#include <utility>
#include <glm/vec2.hpp>

#include "shaders/SpriteRenderer.h"
#include "shaders/TextRenderer.h"
#include "FaceModifier.h"
#include "FaceType.h"
#include "gameobject/dice/Dice.h"

namespace DGR {

class BattleScene;

class Dice;

class Face : public std::enable_shared_from_this<Face> {
protected:
    std::string name;
    std::weak_ptr<Dice> dice{};

    /// level up stats
    int level = 1;
    int bonusValuePerLevel{};

    /// face stats
    int face_;

    int level1DefaultValue{};
    int value{};

    FaceModifier modifiers{};

    bool hover = false;

    static const std::vector<glm::vec2> faceDeltaPos;

    void drawFaceToolTip(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                         const std::unique_ptr<TextRenderer> &textRenderer,
                         glm::vec2 position) const;

    static void applySweepingEdge(const std::shared_ptr<Character> &character, const std::shared_ptr<Face> &face,
                                  const std::shared_ptr<BattleScene> &battleScene, bool isFoe);

public:
    explicit Face(int face_) : face_(face_) {};

    Face(int face_, int value, int bonusValuePerLevel, FaceModifier modifiers = {})
          : bonusValuePerLevel(bonusValuePerLevel), face_(face_), level1DefaultValue(value), value(value),
            modifiers(modifiers) {};

    Face(std::string name, std::weak_ptr<Dice> dice, int face_,
         int value, int bonusValuePerLevel, FaceModifier modifiers);

    /// getters
    [[nodiscard]] std::shared_ptr<Face> getSharedFromThis();

    [[nodiscard]] glm::vec2 getPosition(Dice::dicePos dicePos = Dice::dice_layout_pos) const;

    [[nodiscard]] glm::vec2 getSize() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] int getFace_() const;

    [[nodiscard]] int getValue() const;

    [[nodiscard]] FaceModifier getModifiers() const;

    [[nodiscard]] virtual std::shared_ptr<Face> makeCopy() const = 0;

    [[nodiscard]] virtual std::string getToolTipString() const;

    [[nodiscard]] int getBonusValuePerLevel() const;

    [[nodiscard]] virtual std::string toString() const;

    /// setters
    void setName(const std::string &name_);

    virtual void setValue(int value_);

    virtual void setBonusValuePerLevel(int bonus);

    virtual void addModifier(FaceModifier::modifier modifier);

    virtual void addModifier(const std::string &modifierStr);

    virtual void removeModifier(FaceModifier::modifier modifier);

    virtual void setModifiers(unsigned int modifiers_);

    virtual void setModifiers(FaceModifier modifiers_);

    void setHover(bool hover_);

    void setDice(const std::weak_ptr<Dice> &dice);

    /// functions
    void levelUp();

    virtual bool interactSelf(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene);

    virtual bool interactFriendly(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene);

    virtual bool interactFoe(std::shared_ptr<Character> character, std::shared_ptr<BattleScene> battleScene);

    /// render
    void drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                   const std::unique_ptr<TextRenderer> &textureRenderer,
                   Dice::dicePos dicePos = Dice::dice_layout_pos) const;

    void draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer) const;

    void drawFace(const std::unique_ptr<SpriteRenderer> &spriteRenderer, glm::vec2 position, float rotate = 0.0f,
                  float skewX = 0.0f, float skewY = 0.0f, float height = 1.0f, float width = 1.0f) const;

    void drawCurrentFace(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                         const std::unique_ptr<TextRenderer> &textRenderer,
                         glm::vec2 position) const;

    void drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                   const std::unique_ptr<TextRenderer> &textRenderer,
                   glm::vec2 position) const;

    void drawLevelUpComparison(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                               const std::unique_ptr<TextRenderer> &textRenderer, glm::vec2 position);
};

}


#endif //DICEGONEROGUE_FACE_H
