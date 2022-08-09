//
// Created by thijs on 01-06-22.
//

#ifndef DICEGONEROGUE_FACE_H
#define DICEGONEROGUE_FACE_H

#include <memory>
#include <string>
#include <map>
#include <utility>
#include <glm/vec2.hpp>

#include "shaders/SpriteRenderer.h"
#include "shaders/TextRenderer.h"
#include "FaceModifier.h"
#include "FaceType.h"
#include "Dice.h"

namespace DGR {

class Dice;

class Face : public std::enable_shared_from_this<Face> {
private:
    std::string name;
    std::weak_ptr<Dice> dice{};

    /// level up stats
    int level = 1;
    int bonusValuePerLevel{};

    /// face stats
    int face_{};

    int level1DefaultValue{};
    int value{};
    FaceType type{};
    FaceModifier modifiers{};

    bool hover = false;

    static const std::vector<glm::vec2> faceDeltaPos;
    static const std::vector<glm::vec2> tickValueDeltaPos;

    void drawFaceToolTip(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                         const std::unique_ptr<TextRenderer> &textRenderer,
                         glm::vec2 position) const;

public:
    Face(int face_, int value, int bonusValuePerLevel, FaceType type, FaceModifier modifiers = {})
          : bonusValuePerLevel(bonusValuePerLevel), face_(face_), level1DefaultValue(value), value(value),
            type(type), modifiers(modifiers) {};

    Face(std::string name, std::weak_ptr<Dice> dice, int face_,
         int value, int bonusValuePerLevel, FaceType type, FaceModifier modifiers);

    /// getters
    [[nodiscard]] std::shared_ptr<Face> getSharedFromThis();

    [[nodiscard]] glm::vec2 getPosition(Dice::dicePos dicePos = Dice::dice_layout_pos) const;

    [[nodiscard]] glm::vec2 getSize() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] int getFace_() const;

    [[nodiscard]] int getValue() const;

    [[nodiscard]] FaceType getFaceType() const;

    [[nodiscard]] FaceModifier getModifiers() const;

    [[nodiscard]] std::shared_ptr<Face> makeCopy() const;

    [[nodiscard]] std::string getToolTipString() const;

    /// setters
    void setName(const std::string &name_);

    void setValue(int value_);

    void setBonusValuePerLevel(int bonus);

    void setHover(bool hover_);

    void setType(FaceType type);

    void setType(FaceType::faceType type_);

    void addModifier(FaceModifier::modifier modifier);

    void addModifier(const std::string &modifierStr);

    void removeModifier(FaceModifier::modifier modifier);

    void setModifiers(unsigned int modifiers_);

    void setDice(const std::weak_ptr<Dice> &dice);

    /// functions
    void levelUp();

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
