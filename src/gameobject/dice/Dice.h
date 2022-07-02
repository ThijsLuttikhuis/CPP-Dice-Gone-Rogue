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

class Dice {
public:
    enum dicePos {
        backgroundPos,
        diceLayoutPos,
        currentFacePos
    };

private:
    std::string name;

     std::shared_ptr<Character> character = nullptr;
     std::shared_ptr<Face> faces[6]{};
    bool lock = false;
    bool used = false;

    int currentFace = 0;
    bool hoverCurrentFace = false;

public:
    Dice(std::string name,  std::shared_ptr<Character> character);

    Dice() = default;

    /// getters
    [[nodiscard]]  std::shared_ptr<Face> getCurrentFace() const;

    [[nodiscard]]  std::shared_ptr<Face> getFace(int index) const;

    [[nodiscard]] glm::vec2 getPosition(dicePos dicePos = backgroundPos) const;

    [[nodiscard]] glm::vec2 getSize(dicePos dicePos = backgroundPos) const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos, dicePos dicePos = backgroundPos) const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] bool isLocked() const;

    [[nodiscard]] bool isUsed() const;

    [[nodiscard]]  std::shared_ptr<Dice> makeCopy() const;

    [[nodiscard]]  std::shared_ptr<Character> getCharacter() const;

    /// setters
    void setLocked(bool lock_);

    void setUsed(bool lock_);

    void setCurrentFace(int currentFace_);

    void setCurrentFaceHover(bool hoverCurrentFace_);

    void setFace( std::shared_ptr<Face> face, int index);

    void setCharacter( std::shared_ptr<Character> hero_);

    void setName(const std::string &name_);

    void updateHoverMouse(double xPos, double yPos);

    /// functions
    void roll();

    /// render
    void drawHover( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer);

    void draw( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer);

};

}


#endif //DICEGONEROGUE_DICE_H
