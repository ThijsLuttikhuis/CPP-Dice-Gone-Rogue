//
// Created by thijs on 04-06-22.
//

#include <iostream>
#include <utilities/Utilities.h>

#include "Character.h"
#include "dice/Face.h"
#include "dice/Dice.h"
#include "spell/Spell.h"
#include "GameStateManager.h"
#include "scene/BattleScene.h"

namespace DGR {


Character::Character(std::string name, std::string characterType)
      : GameObject(std::move(name)), characterType(std::move(characterType)) {}

std::shared_ptr<Character> Character::makeCopy(bool copyUniqueID) const {
    auto copy = std::make_shared<Character>(name, getCharacterType());

    copy->setSize(size);
    copy->setPosition(position);

    copy->setMaxHP(maxHP, false);
    copy->setHP(hp);

    copy->setIncomingDamage(incomingDamage);
    copy->setIncomingPoison(incomingPoison);
    copy->setIncomingRegen(incomingRegen);

    copy->setShield(shield);
    copy->setPoison(poison);
    copy->setRegen(regen);
    copy->setBackRow(backRow);
    copy->setDodging(isDodging);
    copy->setUndying(isUndying);

    copy->setSpell(spell->makeCopy());
    copy->getSpell()->setCharacter(copy->getSharedFromThis());

    copy->setDice(dice->makeCopy());
    copy->getDice()->setCharacter(copy->getSharedFromThis());

    if (copyUniqueID) {
        copy->setUniqueID(getUniqueID());
    }
    return copy;
}

std::shared_ptr<Dice> Character::getDice() const {
    return dice;
}

bool Character::getUsedDice() const {
    return dice->isUsed();
}

bool Character::isMouseHovering(double xPos, double yPos, bool alsoCurrentFace) const {
    bool heroHover = Utilities::isPositionInBox(xPos, yPos, position, size);

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
    return characterType;
}

std::shared_ptr<Spell> Character::getSpell() const {
    return spell;
}

void Character::setSpell(const std::shared_ptr<Spell> &spell_) {
    spell = spell_;
}

void Character::setPoison(int poison_) {
    poison = poison_;
}

void Character::setBackRow(bool backRow_) {
    backRow = backRow_;
}

void Character::setRegen(int regen_) {
    regen = regen_;
}

void Character::setDice(const std::shared_ptr<Dice> &dice_) {
    dice = dice_;
}

void Character::setHP(int hp_) {
    hp = hp_;
}

void Character::setMaxHP(int maxHP_, bool setHPToMaxHP) {
    maxHP = maxHP_;
    if (setHPToMaxHP) {
        hp = maxHP_;
    }
}

void Character::setIncomingPoison(int incomingPoison_) {
    incomingPoison = incomingPoison_;
}

void Character::setIncomingRegen(int incomingRegen_) {
    incomingRegen = incomingRegen_;
}

void Character::setUndying(bool isUndying_) {
    isUndying = isUndying_;
}

void Character::setDodging(bool isDodging_) {
    isDodging = isDodging_;
}

void Character::setIncomingDamage(int incomingDamage_) {
    incomingDamage = incomingDamage_;
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

bool Character::interact(const std::shared_ptr<Spell> &clickedSpell,
                         const std::shared_ptr<BattleScene> &battleScene,
                         bool storeAction) {

    bool success = false;

    SpellType type = clickedSpell->getType();

    bool differentCharacterType =
          (getCharacterType() != std::shared_ptr<Character>(clickedSpell->getCharacter())->getCharacterType());

    switch (type.getType()) {
        case SpellType::empty:
            break;
        case SpellType::damage:
            if (differentCharacterType) {
                if (!backRow) {
                    applySpellTypeDamage(clickedSpell, battleScene);
                    success = true;
                }
            }
            break;
        case SpellType::cleanse:
            if (!differentCharacterType) {
                applySpellTypeCleanse(clickedSpell, battleScene);
                success = true;
            }
            break;
        case SpellType::heal:
            break;
        case SpellType::damage_or_shield:
            break;
        case SpellType::heal_or_shield:
            break;
        case SpellType::damage_if_full_health:
            break;
        case SpellType::kill_if_below_threshold:
            break;
    }

    if (success && storeAction) {
        battleScene->getBattleLog()->addAttack(clickedSpell.get(), this);
    }

    if (!success) {
        std::string message = differentCharacterType ? "Please select an ally!" : "Please select an enemy!";

        auto gameStatePtr = std::shared_ptr<GameStateManager>(battleScene->getGameStateManager());
        gameStatePtr->addOnScreenMessage(message);
    }

    return success;
}


bool Character::interact(const std::shared_ptr<Character> &otherCharacter,
                         const std::shared_ptr<BattleScene> &battleScene,
                         bool storeAction) {

    bool success = false;
    std::shared_ptr<Face> face;
    FaceType type;
    bool differentCharacterType = false;

    // single character interactions
    if (!otherCharacter) {
        face = getDice()->getCurrentFace();
        type = face->getFaceType();

        switch (type.getType()) {
            case FaceType::empty:
                success = true;
                break;
            case FaceType::mana:
                battleScene->addMana(face->getValue());
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
        differentCharacterType = (getCharacterType() != otherCharacter->getCharacterType());

        face = otherCharacter->getDice()->getCurrentFace();
        type = face->getFaceType();

        switch (type.getType()) {
            case FaceType::damage:
                if (differentCharacterType) {
                    if (!backRow || (backRow && face->getModifiers().hasModifier(FaceModifier::modifier::ranged))) {
                        applyFaceTypeDamage(face, battleScene);
                        success = true;
                    }
                }
                break;
            case FaceType::heal:
                if (!differentCharacterType) {
                    applyFaceTypeHeal(face, battleScene);
                    success = true;
                }
                break;
            case FaceType::shield:
                if (!differentCharacterType) {
                    applyFaceTypeShield(face, battleScene);
                    success = true;
                }
                break;
            case FaceType::bonus_health:
                if (!differentCharacterType) {
                    applyFaceTypeBonusHealth(face, battleScene);
                    success = true;
                }
                break;
            case FaceType::undying:
                //TODO:
                return false;
            case FaceType::heal_and_shield:
                if (!differentCharacterType) {
                    applyFaceTypeShield(face, battleScene);
                    applyFaceTypeHeal(face, battleScene);
                    success = true;
                }
                break;
            case FaceType::heal_and_mana:
                if (!differentCharacterType) {
                    battleScene->addMana(face->getValue());
                    applyFaceTypeHeal(face, battleScene);
                    success = true;
                }
                break;
            case FaceType::shield_and_mana:
                if (!differentCharacterType) {
                    battleScene->addMana(face->getValue());
                    applyFaceTypeShield(face, battleScene);
                    success = true;
                }
                break;
            case FaceType::damage_and_mana:
                if (differentCharacterType) {
                    if (!backRow || (backRow && face->getModifiers().hasModifier(FaceModifier::modifier::ranged))) {
                        battleScene->addMana(face->getValue());
                        applyFaceTypeDamage(face, battleScene);
                        success = true;
                    }
                }
                break;
            case FaceType::damage_and_self_shield:
                if (differentCharacterType) {
                    otherCharacter->applyFaceTypeShield(face, battleScene);
                    applyFaceTypeDamage(face, battleScene);
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

    if (success && storeAction) {
        battleScene->getBattleLog()->addAttack(this, otherCharacter.get());
    }

    if (!success && otherCharacter) {
        if ((differentCharacterType && getCharacterType() == "enemy") ||
            (!differentCharacterType && getCharacterType() == "hero")) {

            std::string message = differentCharacterType ? "Please select an ally!" : "Please select an enemy!";

            auto gameStatePtr = std::shared_ptr<GameStateManager>(battleScene->getGameStateManager());
            gameStatePtr->addOnScreenMessage(message);
        }
    }
    return success;
}

void Character::setShield(int shield_) {
    shield = shield_;
}

void Character::roll() {
    if (!dice->isLocked()) {
        dice->roll();
    }
}

void Character::applyDamageStep() {
    if (isDead()) {
        return;
    }

    if (!isDodging) {
        incomingDamage -= shield;
        if (incomingDamage > 0) {
            hp -= incomingDamage;
        }
        if (incomingPoison > 0) {
            poison += incomingPoison;
        }
    }
    if (incomingRegen > 0) {
        regen += incomingRegen;
    }

    hp -= poison;
    hp += regen;
    hp = std::min(hp, maxHP);
    hp = std::max(hp, isUndying ? 1 : 0);

    shield = 0;
    incomingDamage = 0;
    incomingPoison = 0;
    incomingRegen = 0;
    isDodging = false;
    isUndying = false;
}

void Character::applySpellTypeDamage(const std::shared_ptr<Spell> &spell_,
                                     const std::shared_ptr<BattleScene> &battleScene) {
    (void) battleScene;

    int value = spell_->getValue();
    incomingDamage += value;
}


void Character::applySpellTypeCleanse(const std::shared_ptr<Spell> &spell_,
                                      const std::shared_ptr<BattleScene> &battleScene) {
    (void) spell_, (void) battleScene;

    poison = 0;
    incomingPoison = 0;
}


void Character::applyFaceTypeDamage(const std::shared_ptr<Face> &face,
                                    const std::shared_ptr<BattleScene> &battleScene) {

    int value = face->getValue();
    FaceModifier modifiers = face->getModifiers();

    if (modifiers.hasModifier(FaceModifier::modifier::first_blood)) {
        if (hp == maxHP && incomingDamage == 0) {
            value *= 2;
        }
    }

    incomingDamage += value;
    if (modifiers.hasModifier(FaceModifier::modifier::poison)) {
        incomingPoison += value;
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applyFaceModifierSweepingEdge(FaceType::damage, face, battleScene);
    }
}

void Character::applyFaceTypeHeal(const std::shared_ptr<Face> &face,
                                  const std::shared_ptr<BattleScene> &battleScene) {

    int value = face->getValue();
    FaceModifier modifiers = face->getModifiers();

    hp += value;
    hp = std::min(hp, maxHP);
    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        applyFaceModifierCleanse(face, battleScene);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::regen)) {
        incomingRegen += value;
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applyFaceModifierSweepingEdge(FaceType::heal, face, battleScene);
    }
}

void Character::applyFaceTypeShield(const std::shared_ptr<Face> &face,
                                    const std::shared_ptr<BattleScene> &battleScene) {

    int value = face->getValue();
    FaceModifier modifiers = face->getModifiers();

    shield += value;
    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        applyFaceModifierCleanse(face, battleScene);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applyFaceModifierSweepingEdge(FaceType::shield, face, battleScene);
    }
}

void Character::applyFaceTypeBonusHealth(const std::shared_ptr<Face> &face,
                                         const std::shared_ptr<BattleScene> &battleScene) {
    int value = face->getValue();
    FaceModifier modifiers = face->getModifiers();

    maxHP += value;
    hp += value;
    if (modifiers.hasModifier(FaceModifier::modifier::cleanse)) {
        applyFaceModifierCleanse(face, battleScene);
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweeping_edge)) {
        applyFaceModifierSweepingEdge(FaceType::bonus_health, face, battleScene);
    }
}

void Character::applyFaceModifierCleanse(const std::shared_ptr<Face> &face,
                                         const std::shared_ptr<BattleScene> &battleScene) {
    (void) face, (void) battleScene;
    poison = 0;
    incomingPoison = 0;
}

void Character::applyFaceModifierSweepingEdge(FaceType::faceType type, const std::shared_ptr<Face> &face,
                                              const std::shared_ptr<BattleScene> &battleScene) {

    face->removeModifier(FaceModifier::modifier::sweeping_edge);
    auto neighbours = battleScene->getNeighbours(shared_from_this());
    for (auto &neighbour : {neighbours.first, neighbours.second}) {
        if (neighbour) {
            switch (type) {
                case FaceType::damage:
                    neighbour->applyFaceTypeDamage(face, battleScene);
                    break;
                case FaceType::heal:
                    neighbour->applyFaceTypeHeal(face, battleScene);
                    break;
                case FaceType::shield:
                    neighbour->applyFaceTypeShield(face, battleScene);
                    break;
                case FaceType::bonus_health:
                    neighbour->applyFaceTypeBonusHealth(face, battleScene);
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

void Character::drawHealthBar(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                              const std::shared_ptr<TextRenderer> &textRenderer) const {
    glm::vec2 hpBarPosition = position + glm::vec2(-6, size.y + 24);
    glm::vec2 hpBarSize = glm::vec2(size.x + 12, 8);
    std::string hpText = std::to_string(hp) + " / " + std::to_string(maxHP);

    spriteRenderer->drawSprite("box", 0.95, hpBarPosition, hpBarSize,
                               1.0f, glm::vec3(0.2f), 0.5f);
    textRenderer->drawText(hpText, 0.5, hpBarPosition, hpBarSize);

    float textDRight = 2;
    float textDUp = 4;
    float dPosRight, dPosUp;
    glm::vec2 position_, textPosition_, size_;
    if (shield > 0) {
        dPosRight = hpBarSize.x * 12.0f / 16.0f - 2.0f;
        dPosUp = -6;
        position_ = hpBarPosition + glm::vec2(dPosRight, dPosUp);
        textPosition_ = hpBarPosition + glm::vec2(dPosRight + textDRight, dPosUp + textDUp);
        size_ = glm::vec2(11, 14);

        spriteRenderer->drawSprite("hero_shield", 0.7, position_, size_,
                                   0.0f, glm::vec3(0.1f), 0.5f);
        textRenderer->drawText(std::to_string(shield), 0.5, textPosition_, size_);
    }

    if (incomingDamage > 0) {
        dPosRight = hpBarSize.x * 12.0f / 16.0f - 2.0f;
        dPosUp = 8;
        position_ = hpBarPosition + glm::vec2(dPosRight, dPosUp);
        textPosition_ = hpBarPosition + glm::vec2(dPosRight + textDRight, dPosUp + textDUp);
        size_ = glm::vec2(11, 14);
        spriteRenderer->drawSprite("hero_damage", 0.7, position_, size_,
                                   90.0f, glm::vec3(0.1f), 0.5f);

        textRenderer->drawText("^" + std::to_string(incomingDamage) + "^", 0.5,
                               textPosition_, size_, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }

    int totalPoison = poison + incomingPoison;
    int totalRegen = regen + incomingRegen;
    if (totalPoison != totalRegen) {
        bool morePoisonThanRegen = totalPoison > totalRegen;
        glm::vec3 color = (morePoisonThanRegen ? FaceModifier(FaceModifier::modifier::poison).toColor()
                                               : FaceModifier(FaceModifier::modifier::regen).toColor()) * 1.5f;

        dPosRight = hpBarSize.x * 8.0f / 16.0f - 10.0f;
        dPosUp = 8;
        position_ = hpBarPosition + glm::vec2(dPosRight, dPosUp);
        textPosition_ = hpBarPosition + glm::vec2(dPosRight + textDRight, dPosUp + textDUp);
        size_ = glm::vec2(11, 14);

        spriteRenderer->drawSprite("hero_mana", 0.7, position_, size_,
                                   morePoisonThanRegen ? 90.0f : 0.0f, color, 0.5f);

        textRenderer->drawText("^" + std::to_string(std::abs(totalPoison - totalRegen)) + "^", 0.5,
                               textPosition_, size_, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }
}

void Character::drawShadow(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                           const std::shared_ptr<TextRenderer> &textRenderer) const {

    (void) textRenderer;

    spriteRenderer->drawSprite("shadow: " + name, 1.0f, position, size * glm::vec2(1.0f, 1.0f));

}

void Character::draw(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                     const std::shared_ptr<TextRenderer> &textRenderer) const {


    spriteRenderer->drawSprite(name, 1.0f, position, size * glm::vec2(1.0f, 1.0f));

    drawHealthBar(spriteRenderer, textRenderer);

    spell->draw(spriteRenderer, textRenderer);
    dice->draw(spriteRenderer, textRenderer);
}

void Character::drawHover(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                          const std::shared_ptr<TextRenderer> &textRenderer) const {
    if (hover) {
#if DGR_DEBUG
        std::cout << "hover: " << getName() << " -- x: " << getPosition().x
                  << " -- y: " << getPosition().y << std::endl;
#endif
        dice->drawHover(spriteRenderer, textRenderer);
    }

    if (spell->getHover()) {
        spell->drawSpellToolTip(spriteRenderer, textRenderer);
    }
}

void Character::drawBox(const std::shared_ptr<SpriteRenderer> &spriteRenderer, glm::vec3 color) const {
    spriteRenderer->drawSprite("box", 0.4f, position, size, 1.0f, color, 0.0f);
}

void GameObject::drawHeroOnly(const std::shared_ptr<SpriteRenderer> &spriteRenderer) const {
    spriteRenderer->drawSprite(name, 1.0f, position, size);
}

std::shared_ptr<Character> Character::getSharedFromThis() {
    return shared_from_this();
}

}
