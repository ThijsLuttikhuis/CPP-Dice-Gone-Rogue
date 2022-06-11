//
// Created by thijs on 10-06-22.
//

#include <sstream>

#include "utilities/Utilities.h"
#include "gameobject/Character.h"
#include "Spell.h"

namespace DGR {

glm::vec2 Spell::getPosition() const {

    glm::vec2 characterPosition = character->getPosition();
    glm::vec2 dPos = glm::vec2(-6, character->getSize().y + 36);
    return characterPosition + dPos;
}

bool Spell::isMouseHovering(double xPos, double yPos) const {
    auto position = getPosition();
    auto size = getSize();
    return Utilities::isPositionInBox(xPos, yPos, position, size);
}

Character* Spell::getCharacter() const {
    return character;
}

SpellType Spell::getType() const {
    return type;
}

int Spell::getValue() const {
    return value;
}

bool Spell::getHover() const {
    return hover;
}

int Spell::getCost() const {
    return cost;
}

glm::vec2 Spell::getSize() const {
    glm::vec2 characterSize = character->getSize();

    return glm::vec2(characterSize.x + 12, 12);
}

void Spell::setHover(bool hover_) {
    hover = hover_;
}

void Spell::setCharacter(Character* character_) {
    character = character_;
}

void Spell::drawSpellToolTip(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    auto position = getPosition();

    int tooltipWidth = 96;
    glm::vec2 tooltipDPos(5, -5);
    glm::vec2 tooltipSize(tooltipWidth, 1);

    glm::vec2 backgroundSize = glm::vec2(tooltipWidth, 16);
    spriteRenderer->drawSprite("box", 0.0f,
                               position + tooltipDPos, backgroundSize, 0.0f, glm::vec3(0.1f), 0.9f);

    std::ostringstream tooltipOSS;
    tooltipOSS << name << ": ";
    if (value != 0) {
        tooltipOSS << value << " ";
    }
    tooltipOSS << type.toString() << " (" << cost << " mana)";

    textRenderer->drawText(tooltipOSS.str(), 0.0f, position + tooltipDPos, tooltipSize);
}

void Spell::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    auto position = getPosition();
    auto size = getSize();

    spriteRenderer->drawSprite(name, 0.0f, position, size);
}

void Spell::drawBox(SpriteRenderer* spriteRenderer, glm::highp_vec3 color) {
    auto position = getPosition();
    auto size = getSize();

    spriteRenderer->drawSprite("box", 0.0f, position, size, 1.0f, color, 0.0f);
}

}
