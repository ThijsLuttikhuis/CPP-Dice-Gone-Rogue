//
// Created by thijs on 04-06-22.
//

#include <iostream>

#include "Character.h"
#include "dice/Face.h"
#include "GameStateManager.h"

namespace DGR {

Character::Character(const std::string &name, glm::vec2 position, glm::vec2 size)
      : GameObject(name, position, size), dice(new Dice(name, this)) {
}

Character* Character::makeUndamagedCopy() const {
    auto* copy = new Character(name);

    copy->setSize(size);
    copy->setPosition(position);

    copy->setMaxHP(maxHP);
    copy->setDice(dice->copy());
    copy->getDice()->setCharacter(copy);

    return nullptr;
}

Dice* Character::getDice() const {
    return dice;
}

bool Character::getUsedDice() const {
    return dice->isUsed();
}

bool Character::isMouseHovering(double xPos, double yPos, bool alsoCurrentFace) const {
    bool heroHover = (xPos > position.x && xPos < position.x + size.x)
                     && (yPos > position.y && yPos < position.y + size.y);

    bool currentFaceHover = false;
    if (alsoCurrentFace) {
        currentFaceHover = dice->isMouseHovering(xPos, yPos, Dice::currentFacePos);
    }
    return heroHover || currentFaceHover;
}

int Character::getIncomingDamage() const {
    return incomingDamage;
}

bool Character::isDead() const {
    return hp <= 0;
}

std::string Character::getCharacterType() const {
    return "character type not set";
}

void Character::setDice(Dice* dice_) {
    dice = dice_;
}

void Character::setMaxHP(int maxHP_, bool setHPToMaxHP) {
    maxHP = maxHP_;
    if (setHPToMaxHP) {
        hp = maxHP_;
    }
}

void Character::setDiceLock(bool diceLock_) {
    dice->setLocked(diceLock_);
}

void Character::toggleDiceLock() {
    dice->setLocked(!dice->isLocked());
}

void Character::setUsedDice(bool usedDice) {
    dice->setUsed(usedDice);
}

bool Character::interact(Character* otherCharacter, GameStateManager* gameState) {
    bool success = false;
    Face* face;
    FaceType type;

    // single character interactions
    if (!otherCharacter) {
        face = getDice()->getCurrentFace();
        type = face->getFaceType();

        switch (type.getType()) {
            case FaceType::empty:
                success = true;
                break;
            case FaceType::mana:
                gameState->addMana(face->getValue());
                success = true;
                break;
            case FaceType::dodge:
                isDodging = true;
                success = true;
                break;
            default:
                break;
        }
    } else {
        // interaction with another character of the same type or not
        bool differentCharacterType = (getCharacterType() != otherCharacter->getCharacterType());

        face = otherCharacter->getDice()->getCurrentFace();
        type = face->getFaceType();

        switch (type.getType()) {
            case FaceType::damage:
                if (differentCharacterType) {
                    if (!backRow || (backRow && face->getModifiers().hasModifier(FaceModifier::modifier::ranged))) {
                        applyFaceTypeDamage(face, gameState);
                        success = true;
                    }
                }
                break;
            case FaceType::heal:
                if (!differentCharacterType) {
                    applyFaceTypeHeal(face, gameState);
                    success = true;
                }
                break;
            case FaceType::shield:
                if (!differentCharacterType) {
                    applyFaceTypeShield(face, gameState);
                    success = true;
                }
                break;
            case FaceType::undying:
                //TODO:
                return false;
            case FaceType::heal_and_shield:
                if (!differentCharacterType) {
                    applyFaceTypeShield(face, gameState);
                    applyFaceTypeHeal(face, gameState);
                    success = true;
                }
                break;
            case FaceType::heal_and_mana:
                if (!differentCharacterType) {
                    gameState->addMana(face->getValue());
                    applyFaceTypeHeal(face, gameState);
                    success = true;
                }
                break;
            case FaceType::shield_and_mana:
                if (!differentCharacterType) {
                    gameState->addMana(face->getValue());
                    applyFaceTypeShield(face, gameState);
                    success = true;
                }
                break;
            case FaceType::damage_and_mana:
                if (differentCharacterType) {
                    if (!backRow || (backRow && face->getModifiers().hasModifier(FaceModifier::modifier::ranged))) {
                        gameState->addMana(face->getValue());
                        applyFaceTypeDamage(face, gameState);
                        success = true;
                    }
                }
                break;
            case FaceType::damage_and_self_shield:
                if (differentCharacterType) {
                    otherCharacter->applyFaceTypeShield(face, gameState);
                    applyFaceTypeDamage(face, gameState);
                    success = true;
                }
                break;
            default:
                return false;

        }
    }
    if (success) {
        if (face->getModifiers().hasModifier(FaceModifier::modifier::growth)) {
            face->setValue(face->getValue() + 1);
        }
        if (face->getModifiers().hasModifier(FaceModifier::modifier::decay)) {
            face->setValue(face->getValue() - 1);
        }
        if (face->getModifiers().hasModifier(FaceModifier::modifier::single_use)) {
            face->setModifiers(0);
            face->setType(FaceType::empty);
            face->setValue(0);
        }
    }
    return success;
}

void Character::roll() {
    if (!dice->isLocked()) {
        dice->roll();
    }
}

void Character::applyDamageStep() {

    if (!isDodging) {
        incomingDamage -= shield;
        if (incomingDamage > 0) {
            hp -= incomingDamage;
        }
    }
    hp -= poison;
    hp += regen;
    hp = std::min(hp, maxHP);
    hp = std::max(hp, isUndying ? 1 : 0);

    shield = 0;
    incomingDamage = 0;
    isDodging = false;
    isUndying = false;
}


void Character::applyFaceTypeDamage(Face* face, GameStateManager* gameState) {
    int value = face->getValue();
    FaceModifier modifiers = face->getModifiers();

    if (modifiers.hasModifier(FaceModifier::modifier::first_blood)) {
        if (hp == maxHP && incomingDamage == 0) {
            value *= 2;
        }
    }

    incomingDamage += value;
    if (modifiers.hasModifier(FaceModifier::modifier::poison)) {
        poison += value;
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applyFaceModifierSweepingEdge(FaceType::damage, face, gameState);
    }

}

void Character::applyFaceTypeHeal(Face* face, GameStateManager* gameState) {
    int value = face->getValue();
    FaceModifier modifiers = face->getModifiers();

    hp += value;
    hp = std::min(hp, maxHP);
    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        applyFaceModifierCleanse(face, gameState);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applyFaceModifierSweepingEdge(FaceType::heal, face, gameState);
    }
}

void Character::applyFaceTypeShield(Face* face, GameStateManager* gameState) {
    int value = face->getValue();
    FaceModifier modifiers = face->getModifiers();

    shield += value;
    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        applyFaceModifierCleanse(face, gameState);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applyFaceModifierSweepingEdge(FaceType::shield, face, gameState);
    }
}

void Character::applyFaceModifierCleanse(Face* face, GameStateManager* gameState) {
    (void) face, (void) gameState;
    poison = 0;
}

void Character::applyFaceModifierSweepingEdge(FaceType::faceType type, Face* face, GameStateManager* gameState) {
    face->removeModifier(FaceModifier::modifier::sweeping_edge);
    auto neighbours = gameState->getNeighbours(this);
    for (auto &neighbour : {neighbours.first, neighbours.second}) {
        if (neighbour) {
            switch (type) {
                case FaceType::damage:
                    neighbour->applyFaceTypeDamage(face, gameState);
                    break;
                case FaceType::heal:
                    neighbour->applyFaceTypeHeal(face, gameState);
                    break;
                case FaceType::shield:
                    neighbour->applyFaceTypeShield(face, gameState);
                    break;
                case FaceType::undying:
                    //TODO:
                    break;
                default:
                    break;
            }
        }
    }
    face->addModifier(FaceModifier::modifier::sweeping_edge);
}

void Character::drawHealthBar(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    glm::vec2 hpBarPosition = position + glm::vec2(-6, size.y + 24);
    glm::vec2 hpBarSize = glm::vec2(size.x + 12, 8);
    std::string hpText = std::to_string(hp) + " / " + std::to_string(maxHP);

    spriteRenderer->drawSprite("box", 0.95, hpBarPosition, hpBarSize,
                               1.0f, glm::vec3(0.2f), 0.5f);
    textRenderer->drawText(hpText, 0.5, hpBarPosition, hpBarSize);

    float textDRight = 2;
    float textDUp = 4;

    if (shield > 0) {
        float shieldPosRight = -size.x * 6.0f / 16.0f;
        float shieldPosUp = -6;
        glm::vec2 shieldPosition =
              hpBarPosition + glm::vec2(hpBarSize.x + shieldPosRight, shieldPosUp);
        glm::vec2 shieldTextPosition =
              hpBarPosition + glm::vec2(hpBarSize.x + shieldPosRight + textDRight, shieldPosUp + textDUp);

        glm::vec2 shieldSize = glm::vec2(11, 14);
        spriteRenderer->drawSprite("hero_shield", 0.7, shieldPosition, shieldSize,
                                   0.0f, glm::vec3(0.1f), 0.5f);

        textRenderer->drawText(std::to_string(shield), 0.5, shieldTextPosition, shieldSize);
    }
    if (incomingDamage > 0) {
        float incDamagePosRight = -size.x * 6.0f / 16.0f;
        float incDamagePosUp = 6;
        glm::vec2 incomingDamagePosition =
              hpBarPosition + glm::vec2(hpBarSize.x + incDamagePosRight, incDamagePosUp);
        glm::vec2 incomingDamageTextPosition =
              hpBarPosition + glm::vec2(hpBarSize.x + incDamagePosRight + textDRight, incDamagePosUp + textDUp);

        glm::vec2 incomingDamageSize = glm::vec2(11, 14);
        spriteRenderer->drawSprite("hero_damage", 0.7, incomingDamagePosition, incomingDamageSize,
                                   90.0f, glm::vec3(0.1f), 0.5f);

        textRenderer->drawText("^" + std::to_string(incomingDamage) + "^", 0.5,
                               incomingDamageTextPosition, incomingDamageSize, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }

    if (poison > 0) {
        float poisonPosRight = -size.x * 22.0f / 16.0f;
        float poisonPosUp = 6;
        glm::vec2 incomingDamagePosition =
              hpBarPosition + glm::vec2(hpBarSize.x + poisonPosRight, poisonPosUp);
        glm::vec2 incomingDamageTextPosition =
              hpBarPosition + glm::vec2(hpBarSize.x + poisonPosRight + textDRight, poisonPosUp + textDUp);

        glm::vec2 incomingDamageSize = glm::vec2(11, 14);
        spriteRenderer->drawSprite("hero_mana", 0.7, incomingDamagePosition, incomingDamageSize,
                                   90.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);

        textRenderer->drawText("^" + std::to_string(poison) + "^", 0.5,
                               incomingDamageTextPosition, incomingDamageSize, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }
}

void Character::draw(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    spriteRenderer->drawSprite(name, 1.0f, position, size);

    drawHealthBar(spriteRenderer, textRenderer);

    dice->draw(spriteRenderer, textRenderer);
}

void Character::drawHover(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    if (hover) {
#if DEBUG
        std::cout << "hover: " << getName() << " -- x: " << getPosition().x
                  << " -- y: " << getPosition().y << std::endl;
#endif
        dice->drawHover(spriteRenderer, textRenderer);
    }
}

void Character::drawBox(SpriteRenderer* spriteRenderer, glm::vec3 color) {
    spriteRenderer->drawSprite("box", 0.4f, position, size, 1.0f, color, 0.0f);
}

}
