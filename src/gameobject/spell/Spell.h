//
// Created by thijs on 10-06-22.
//

#ifndef DICEGONEROGUE_SPELL_H
#define DICEGONEROGUE_SPELL_H

#include <string>
#include <glm/vec2.hpp>
#include "SpellType.h"

namespace DGR {

class Character;

class Spell {
private:
    std::string name;

    Character* character = nullptr;

    int cost{};
    int value{};
    SpellType type{};

    bool hover = false;

public:
    Spell(std::string name, Character* character) : name(std::move(name)), character(character) {}

    Spell(std::string name, int cost, int value, SpellType type)
    : name(std::move(name)), cost(cost), value(value), type(type) {}

    Spell() = default;

    /// getters
    [[nodiscard]] glm::vec2 getPosition() const;

    [[nodiscard]] glm::vec2 getSize() const;

    [[nodiscard]] bool getHover() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] int getCost() const;

    [[nodiscard]] Character* getCharacter() const;

    [[nodiscard]] SpellType getType() const;

    [[nodiscard]] int getValue() const;

    /// setters
    void setCharacter(Character* character_);

    void setHover(bool hover_);

    /// render
    void draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    void drawSpellToolTip(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer);

    void drawBox(SpriteRenderer* spriteRenderer, glm::highp_vec3 color);

};

}


#endif //DICEGONEROGUE_SPELL_H
