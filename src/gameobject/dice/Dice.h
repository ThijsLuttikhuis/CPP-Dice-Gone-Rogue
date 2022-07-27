//
// Created by thijs on 31-05-22.
//

#ifndef DICEGONEROGUE_DICE_H
#define DICEGONEROGUE_DICE_H


#include <string>
#include <utility>

#include "shaders/TextRenderer.h"
#include "shaders/SpriteRenderer.h"
#include "FaceModifier.h"
#include "FaceType.h"

namespace DGR {

class Character;

class Face;

class Dice : public std::enable_shared_from_this<Dice> {
public:
    enum dicePos {
        background_pos,
        dice_layout_pos,
        current_face_pos,
    };

private:
    std::string name;

    std::weak_ptr<Character> character{};
    std::shared_ptr<Face> faces[6]{};
    bool lock = false;
    bool used = false;

    int currentFace = 0;
    bool hoverCurrentFace = false;

public:
    Dice() = default;

    /// getters
    [[nodiscard]] std::shared_ptr<Dice> getSharedFromThis();

    [[nodiscard]]  std::shared_ptr<Face> getCurrentFace() const;

    [[nodiscard]]  std::shared_ptr<Face> getFace(int index) const;

    [[nodiscard]] glm::vec2 getPosition(dicePos dicePos = background_pos) const;

    [[nodiscard]] glm::vec2 getSize(dicePos dicePos = background_pos) const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, dicePos dicePos = background_pos) const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] bool isLocked() const;

    [[nodiscard]] bool isUsed() const;

    [[nodiscard]]  std::shared_ptr<Dice> makeCopy() const;

    /// setters
    void setLocked(bool lock_);

    void setUsed(bool lock_);

    void setCurrentFace(int currentFace_);

    void setCurrentFaceHover(bool hoverCurrentFace_);

    void setFace(const std::shared_ptr<Face> &face, int index);

    void setCharacter(const std::weak_ptr<Character> &character_);

    void setName(const std::string &name_);

    void updateHoverMouse(double xPos, double yPos);

    /// functions
    void roll();

    /// render
    void drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                   const std::unique_ptr<TextRenderer> &textRenderer, bool drawCurrentFaceIndicator) const;

    void draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
              const std::unique_ptr<TextRenderer> &textRenderer);

};

}


#endif //DICEGONEROGUE_DICE_H
