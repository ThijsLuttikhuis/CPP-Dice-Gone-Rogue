//
// Created by thijs on 10-06-22.
//

#ifndef DICEGONEROGUE_SPELL_H
#define DICEGONEROGUE_SPELL_H

#include <string>
#include <utility>
#include <glm/vec2.hpp>
#include "SpellType.h"

namespace DGR {

class Character;

class Spell : public std::enable_shared_from_this<Spell> {
private:
    std::string name;

    std::weak_ptr<Character> character{};

    int cost{};
    int value{};
    SpellType type{};

    bool hover = false;

public:
    Spell(std::string name, std::weak_ptr<Character> character)
          : name(std::move(name)), character(std::move(character)) {}

    Spell(std::string name, int cost, int value, SpellType type)
          : name(std::move(name)), cost(cost), value(value), type(type) {}

    Spell() = default;

    /// getters
    [[nodiscard]] glm::vec2 getPosition() const;

    [[nodiscard]] glm::vec2 getSize() const;

    [[nodiscard]] bool getHover() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] int getCost() const;

    [[nodiscard]]  std::weak_ptr<Character> getCharacter() const;

    [[nodiscard]] SpellType getType() const;

    [[nodiscard]] int getValue() const;

    [[nodiscard]] std::shared_ptr<Spell> makeCopy() const;

    /// setters
    void setCharacter(const std::weak_ptr<Character> &character_);

    void setHover(bool hover_);

    /// render
    void draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
              const std::unique_ptr<TextRenderer> &textRenderer);

    void drawSpellToolTip(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                          const std::unique_ptr<TextRenderer> &textRenderer);

    void drawBox(const std::unique_ptr<SpriteRenderer> &spriteRenderer, glm::vec3 color);

};

}


#endif //DICEGONEROGUE_SPELL_H
