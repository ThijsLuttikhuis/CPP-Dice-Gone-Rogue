//
// Created by thijs on 10-06-22.
//

#include <sstream>

#include "utilities/Utilities.h"
#include "gameobject/Character.h"
#include "Spell.h"

namespace DGR {

std::shared_ptr<Spell> Spell::makeCopy() const {
    auto copy = std::make_shared<Spell>(name, cost, value, type);
    copy->setCharacter(character);

    return copy;
}

glm::vec2 Spell::getPosition() const {
    auto characterPtr = std::shared_ptr<Character>(character);

    glm::vec2 characterPosition = characterPtr->getPosition();
    glm::vec2 dPos = glm::vec2(-6, characterPtr->getSize().y + 48);
    return characterPosition + dPos;
}

bool Spell::isMouseHovering(double xPos, double yPos) const {
    if (type == SpellType::empty) {
        return false;
    }

    auto position = getPosition();
    auto size = getSize();
    return Utilities::isPositionInBox(xPos, yPos, position, size);
}

std::weak_ptr<Character> Spell::getCharacter() const {
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
    glm::vec2 characterSize = std::shared_ptr<Character>(character)->getSize();

    return glm::vec2(characterSize.x + 12, 20);
}

void Spell::setHover(bool hover_) {
    hover = hover_;
}

void Spell::setCharacter(const std::weak_ptr<Character> &character_) {
    character = character_;
}

void
Spell::drawSpellToolTip(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                        const std::shared_ptr<TextRenderer> &textRenderer) {
    if (type == SpellType::empty) {
        return;
    }

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

void
Spell::draw(const std::shared_ptr<SpriteRenderer> &spriteRenderer, const std::shared_ptr<TextRenderer> &textRenderer) {

    if (type == SpellType::empty) {
        return;
    }

    auto position = getPosition();
    auto size = getSize();
    spriteRenderer->drawSprite("box", 0.1f, position, size,
                               1.0f, glm::vec3(0.8f), 0.2f);

    auto textPosition = getPosition();
    auto textSize = glm::vec2(getSize().x, 8);
    spriteRenderer->drawSprite("box", 0.0f, textPosition, textSize,
                               0.0f, glm::vec3(0.8f), 0.2f);
    textRenderer->drawText(name, 0.00f, textPosition, textSize);

    float manaCostWidth = 11.0f;
    float spriteHeight = 12.0f;

    auto texturePosition = glm::vec2(getPosition().x - 1, getPosition().y + 8);
    auto textureSize = glm::vec2(getSize().x + 1 - manaCostWidth, spriteHeight);
    spriteRenderer->drawSprite(name, 0.00f, texturePosition, textureSize);

    auto manaCostPosition = glm::vec2(getPosition().x + getSize().x - manaCostWidth, getPosition().y + 8);
    auto manaCostTextPosition =
          glm::vec2(getPosition().x + getSize().x - manaCostWidth - 1, getPosition().y + 12);

    auto manaCostSize = glm::vec2(manaCostWidth, spriteHeight);
    spriteRenderer->drawSprite("mana_small", 0.00f, manaCostPosition, manaCostSize, 0.0f,
                               glm::vec3{1.0f}, 0.6f);

    textRenderer->drawText("^" + std::to_string(cost) + "^", 0.0f, manaCostTextPosition, textSize,
                           Utilities::color2Vec3("0xd4a5fa"));

}

void Spell::drawBox(const std::shared_ptr<SpriteRenderer> &spriteRenderer, glm::highp_vec3 color) {
    if (type == SpellType::empty) {
        return;
    }

    auto position = getPosition();
    auto size = getSize();

    spriteRenderer->drawSprite("box", 0.0f, position, size, 1.0f, color, 0.0f);
}


}
