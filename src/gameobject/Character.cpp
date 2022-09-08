//
// Created by thijs on 04-06-22.
//

#include <iostream>
#include <utility>
#include <utilities/Utilities.h>

#include "Character.h"
#include "gameobject/dice/face/Face.h"
#include "dice/Dice.h"
#include "spell/Spell.h"
#include "GameStateManager.h"
#include "scene/BattleScene.h"

namespace DGR {


Character::Character(std::string name, std::string characterType)
      : Button(std::move(name), glm::vec2(0), glm::vec2(1), glm::vec3(1.0f)),
        characterType(std::move(characterType)) {}

std::shared_ptr<Character> Character::makeCopy(bool copyUniqueID) const {
    auto copy = std::make_shared<Character>(name, getCharacterType());

    copy->setSize(size);
    copy->setPosition(position);

    copy->setXP(xp);
    copy->setLevel(characterLevel);
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

std::shared_ptr<Character> Character::getSharedFromThis() {
    return shared_from_this();
}

std::shared_ptr<Dice> Character::getDice() const {
    return dice;
}

bool Character::getUsedDice() const {
    return dice->isUsed();
}

int Character::getLevel() const {
    return characterLevel;
}

int Character::xpToLevelUp(int currentLevel) const {
    const int baseXP = 48;
    return baseXP * (int) std::pow((currentLevel + 1), 2.5);
}

bool Character::isMouseHovering_(double xPos, double yPos, hoverType hoverType) const {
    switch (hoverType) {
        case onlyHero:
            return Utilities::isPositionInBox(xPos, yPos, position, size);
        case onlyCurrentFace:
            return dice->isMouseHovering(xPos, yPos, Dice::current_face_pos);
        case heroAndCurrentFace:
            return Utilities::isPositionInBox(xPos, yPos, position, size) ||
                   dice->isMouseHovering(xPos, yPos, Dice::current_face_pos);
        case extendedBox:
            return Utilities::isPositionInBox(xPos, yPos, position - glm::vec2(6, 0),
                                              glm::vec2(size.x + 12,
                                                        dice->getPosition(Dice::current_face_pos).y +
                                                        dice->getSize(Dice::current_face_pos).y -
                                                        position.y));
        default:
            return false;
    }
}

bool Character::canLevelUp() const {
    return xp >= xpToLevelUp(characterLevel);
}

void Character::addXP(int xp_) {
    xp += xp_;
}

void Character::setXP(int xp_) {
    xp = xp_;
}

void Character::setLevel(int level) {
    characterLevel = level;
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

int Character::getMaxHpAtLevel(int level) {
    return (int) (level1DefaultMaxHP * std::pow(hpFactorPerLevel, level - 1));
}

double Character::getXPBarFill(double percent) const {
    int xpRequired = xpToLevelUp(characterLevel);
    return (percent / 100.0) * ((double) xp / xpRequired);
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
    hp = std::max(hp, maxHP);
}

void Character::setMaxHP(int maxHP_, bool setHPToMaxHP) {
    level1DefaultMaxHP = maxHP_;
    maxHP = maxHP_;
    if (setHPToMaxHP) {
        hp = maxHP_;
    }
}

void Character::addMaxHP(int dMaxHP, bool alsoAddHP) {
    maxHP += dMaxHP;
    if (alsoAddHP) {
        hp += dMaxHP;
    }
}


void Character::setIncomingPoison(int incomingPoison_) {
    incomingPoison = incomingPoison_;
}

void Character::addIncomingPoison(int dIncomingPoison) {
    incomingPoison += dIncomingPoison;
}

void Character::setIncomingRegen(int incomingRegen_) {
    incomingRegen = incomingRegen_;
}

void Character::addIncomingRegen(int dIncomingRegen) {
    incomingRegen += dIncomingRegen;
}

void Character::setUndying(bool isUndying_) {
    isUndying = isUndying_;
}

void Character::setDodging(bool isDodging_) {
    isDodging = isDodging_;
}

void Character::setItem(Item::itemSlot itemSlot, std::shared_ptr<Item> item) {
    items[itemSlot] = std::move(item);
}

void Character::setIncomingDamage(int incomingDamage_) {
    incomingDamage = incomingDamage_;
}

void Character::addIncomingDamage(int dIncomingDamage) {
    incomingDamage += dIncomingDamage;
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

void Character::levelUp() {
    if (!canLevelUp()) {
        std::cerr << "Character::levelUp: error: not enough xp to level up!!" << std::endl;
        return;
    }

    xp -= xpToLevelUp(characterLevel);
    characterLevel++;
    hp = maxHP = getMaxHpAtLevel(characterLevel);
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

    bool differentCharacterType = false;
    std::shared_ptr<Face> face;
    bool success;

    if (!otherCharacter) {
        face = getDice()->getCurrentFace();

        success = face->interactSelf(getSharedFromThis(), battleScene);
    } else {
        face = otherCharacter->getDice()->getCurrentFace();

        differentCharacterType = (getCharacterType() != otherCharacter->getCharacterType());
        if (differentCharacterType) {
            success = face->interactFoe(getSharedFromThis(), battleScene);
        } else {
            success = face->interactFriendly(getSharedFromThis(), battleScene);
        }
    }



    // single character interactions
//    if (!otherCharacter) {
//        face = getDice()->getCurrentFace();
//        if (face->interact()) {
//
//
//
//
//        face = getDice()->getCurrentFace();
//        type = face->getFaceType();
//
//        switch (type.getType()) {
//            case FaceType::empty:
//                success = true;
//                break;
//            case FaceType::mana:
//                battleScene->addMana(face->getValue());
//                success = true;
//                break;
//            case FaceType::dodge:
//                isDodging = true;
//                success = true;
//                break;
//            default:
//                break;
//        }
//    } else {
//        // interaction with another character of the same type or not
//        differentCharacterType = (getCharacterType() != otherCharacter->getCharacterType());
//
//        face = otherCharacter->getDice()->getCurrentFace();
//        type = face->getFaceType();
//
//        switch (type.getType()) {
//            case FaceType::damage:
//                if (differentCharacterType) {
//                    if (!backRow || (backRow && face->getModifiers().hasModifier(FaceModifier::modifier::ranged))) {
//                        applyFaceTypeDamage(face, battleScene);
//                        success = true;
//                    }
//                }
//                break;
//            case FaceType::heal:
//                if (!differentCharacterType) {
//                    applyFaceTypeHeal(face, battleScene);
//                    success = true;
//                }
//                break;
//            case FaceType::shield:
//                if (!differentCharacterType) {
//                    applyFaceTypeShield(face, battleScene);
//                    success = true;
//                }
//                break;
//            case FaceType::bonus_health:
//                if (!differentCharacterType) {
//                    applyFaceTypeBonusHealth(face, battleScene);
//                    success = true;
//                }
//                break;
//            case FaceType::undying:
//                //TODO:
//                return false;
//            case FaceType::heal_and_shield:
//                if (!differentCharacterType) {
//                    applyFaceTypeShield(face, battleScene);
//                    applyFaceTypeHeal(face, battleScene);
//                    success = true;
//                }
//                break;
//            case FaceType::heal_and_mana:
//                if (!differentCharacterType) {
//                    battleScene->addMana(face->getValue());
//                    applyFaceTypeHeal(face, battleScene);
//                    success = true;
//                }
//                break;
//            case FaceType::shield_and_mana:
//                if (!differentCharacterType) {
//                    battleScene->addMana(face->getValue());
//                    applyFaceTypeShield(face, battleScene);
//                    success = true;
//                }
//                break;
//            case FaceType::damage_and_mana:
//                if (differentCharacterType) {
//                    if (!backRow || (backRow && face->getModifiers().hasModifier(FaceModifier::modifier::ranged))) {
//                        battleScene->addMana(face->getValue());
//                        applyFaceTypeDamage(face, battleScene);
//                        success = true;
//                    }
//                }
//                break;
//            case FaceType::damage_and_self_shield:
//                if (differentCharacterType) {
//                    otherCharacter->applyFaceTypeShield(face, battleScene);
//                    applyFaceTypeDamage(face, battleScene);
//                    success = true;
//                }
//                break;
//            default:
//                return false;
//
//        }
//    }
    if (success) {
        if (face->getModifiers().hasModifier(FaceModifier::modifier::growth)) {
            face->setValue(face->getValue() + 1);
        }
        if (face->getModifiers().hasModifier(FaceModifier::modifier::decay)) {
            face->setValue(face->getValue() - 1);
        }
        if (face->getModifiers().hasModifier(FaceModifier::modifier::single_use)) {
            face->setModifiers(0);
            //face->setType(FaceType::empty);
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


void Character::addShield(int dShield) {
    shield += dShield;
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

    if (modifiers.hasModifier(FaceModifier::modifier::backstab)) {
        if (hp == maxHP && incomingDamage == 0) {
            value *= 2;
        }
    }

    incomingDamage += value;
    if (modifiers.hasModifier(FaceModifier::modifier::poison)) {
        incomingPoison += value;
    }

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
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

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
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

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
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

    if (modifiers.hasModifier(FaceModifier::modifier::sweep)) {
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

    face->removeModifier(FaceModifier::modifier::sweep);
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
    face->addModifier(FaceModifier::modifier::sweep);
}

void Character::drawHealthBar(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                              const std::unique_ptr<TextRenderer> &textRenderer) const {
    glm::vec2 hpBarPosition = position + glm::vec2(-6, size.y + 24);
    glm::vec2 hpBarSize = glm::vec2(size.x + 12, 8);
    std::string hpText = std::to_string(hp) + " / " + std::to_string(maxHP);

    auto color = glm::vec3(0.2f);
    float alpha = 0.5f;
    float edgeAlpha = 1.0f;

    spriteArgs args = {{"color",     &color},
                       {"alpha",     &alpha},
                       {"edgeAlpha", &edgeAlpha}};

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.95, hpBarPosition, hpBarSize, args);
    textRenderer->drawText(hpText, 0.5, hpBarPosition, hpBarSize);

    float textDRight = 2;
    float textDUp = 4;
    float dPosRight, dPosUp;
    glm::vec2 position_, textPosition_, size_;
    if (shield > 0 || isDodging) {
        dPosRight = hpBarSize.x * 12.0f / 16.0f - 2.0f;
        dPosUp = -6;
        position_ = hpBarPosition + glm::vec2(dPosRight, dPosUp);
        textPosition_ = hpBarPosition + glm::vec2(dPosRight + textDRight, dPosUp + textDUp);
        size_ = glm::vec2(11, 14);

        std::string textureName = isDodging ? "hero_dodge" : "hero_shield";

        spriteRenderer->drawSprite(textureName, 0.7, position_, size_,
                                   glm::vec3(0.1f), 0.5f);
        if (!isDodging) {
            textRenderer->drawText(std::to_string(shield), 0.5, textPosition_, size_);
        }
    }

    if (incomingDamage > 0) {
        dPosRight = hpBarSize.x * 12.0f / 16.0f - 2.0f;
        dPosUp = 8;
        position_ = hpBarPosition + glm::vec2(dPosRight, dPosUp);
        textPosition_ = hpBarPosition + glm::vec2(dPosRight + textDRight, dPosUp + textDUp);
        size_ = glm::vec2(11, 14);

        color = glm::vec3(0.1f);
        alpha = 0.5f;
        float rotate = 90.0f;

        args = {{"color",  &color},
                {"alpha",  &alpha},
                {"rotate", &rotate}};

        spriteRenderer->drawSprite("hero_damage", 0.7, position_, size_, args);

        textRenderer->drawText("^" + std::to_string(incomingDamage) + "^", 0.5,
                               textPosition_, size_, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }

    int totalPoison = poison + incomingPoison;
    int totalRegen = regen + incomingRegen;
    if (totalPoison != totalRegen) {
        bool morePoisonThanRegen = totalPoison > totalRegen;

        dPosRight = hpBarSize.x * 8.0f / 16.0f - 10.0f;
        dPosUp = 8;
        position_ = hpBarPosition + glm::vec2(dPosRight, dPosUp);
        textPosition_ = hpBarPosition + glm::vec2(dPosRight + textDRight, dPosUp + textDUp);
        size_ = glm::vec2(11, 14);

        float rotate = morePoisonThanRegen ? 90.0f : 0.0f;
        color = (morePoisonThanRegen ? FaceModifier(FaceModifier::modifier::poison).toColor()
                                     : FaceModifier(FaceModifier::modifier::regen).toColor()) * 1.5f;
        alpha = 0.5f;

        args = {{"color",  &color},
                {"alpha",  &alpha},
                {"rotate", &rotate}};

        spriteRenderer->drawSprite("hero_mana", 0.7, position_, size_, args);

        textRenderer->drawText("^" + std::to_string(std::abs(totalPoison - totalRegen)) + "^", 0.5,
                               textPosition_, size_, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }
}

void Character::drawShadow(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                           const std::unique_ptr<TextRenderer> &textRenderer) const {
    (void) textRenderer;

    spriteArgs args = {};
    spriteRenderer->drawSprite(SpriteRenderer::shadow, name, 1.0f, position, size, args);
}

void Character::draw(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                     const std::unique_ptr<TextRenderer> &textRenderer) const {

    spriteRenderer->drawSprite(name, 1.0f, position, size);

    drawHealthBar(spriteRenderer, textRenderer);

    spell->draw(spriteRenderer, textRenderer);
    dice->draw(spriteRenderer, textRenderer);
}

void Character::drawHover(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                          const std::unique_ptr<TextRenderer> &textRenderer, bool drawCurrentFaceIndicator) const {
    if (hover) {
        dice->drawHover(spriteRenderer, textRenderer, drawCurrentFaceIndicator);
    }

    if (spell->getHover()) {
        spell->drawSpellToolTip(spriteRenderer, textRenderer);
    }
}

void Character::drawBox(const std::unique_ptr<SpriteRenderer> &spriteRenderer, glm::vec3 color) const {
    float alpha = 0.0f;
    float edgeAlpha = 1.0f;

    spriteArgs args = {{"color",     &color},
                       {"alpha",     &alpha},
                       {"edgeAlpha", &edgeAlpha}};

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.4f, position, size, args);
}

void Character::drawHeroOnly(const std::unique_ptr<SpriteRenderer> &spriteRenderer) const {
    spriteRenderer->drawSprite(name, 1.0f, position, size);
}

void Character::drawXPBar(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                          const std::unique_ptr<TextRenderer> &textRenderer, double xpPercent) {

    int xpBarHeight = 8;
    auto barPosition = position + glm::vec2(0, size.y + 4);
    auto barSize = glm::vec2(size.x, xpBarHeight);

    double barFill = getXPBarFill(xpPercent);

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 1.0f,
                               barPosition, barSize, glm::vec3(0.7));

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.9f,
                               barPosition, barSize * glm::vec2(barFill, 1.0), glm::vec3(0.5f, 0.2f, 0.2f));

    textRenderer->drawText("Lvl " + std::to_string(characterLevel), 0.1f, barPosition, barSize);

}

void Character::drawLevelUp(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                            const std::unique_ptr<TextRenderer> &textRenderer) {

    drawHealthBar(spriteRenderer, textRenderer);

    glm::vec2 deltaHPBarPosition = position + glm::vec2(-6, size.y + 24) + glm::vec2(size.x + 12, 0);
    glm::vec2 deltaHPBarSize = glm::vec2(20, 8);
    std::string deltaHPText = "^+ " +
                              std::to_string(getMaxHpAtLevel(characterLevel) -
                                             getMaxHpAtLevel(characterLevel - 1)) + "^";

    auto color = glm::vec3(0.2f);
    float alpha = 0.5f;
    float edgeAlpha = 1.0f;

    spriteArgs args = {{"color",     &color},
                       {"alpha",     &alpha},
                       {"edgeAlpha", &edgeAlpha}};

    spriteRenderer->drawSprite(SpriteRenderer::box, "", 0.95,
                               deltaHPBarPosition, deltaHPBarSize, args);

    textRenderer->drawText(deltaHPText, 0.1f, deltaHPBarPosition, deltaHPBarSize,
                           glm::vec3(0.1f, 0.5f, 0.1f));

}

void Character::drawKey(const std::unique_ptr<SpriteRenderer> &spriteRenderer,
                        const std::unique_ptr<TextRenderer> &textRenderer) const {
    (void) spriteRenderer;

    if (keyboardKey >= 0) {
        std::string keyName = Utilities::keyPressToName(keyboardKey);

        if (characterType == "hero") {
            keyName = "^" + keyName + "^";
        } else {
            keyName = "^Shift+" + keyName + "^";
        }

        glm::vec2 keyPressSize = glm::vec2(keyName.length() * 4 - 2, 8);

        textRenderer->drawText(keyName, 0.0f,
                               position + glm::vec2(0.0f, -keyPressSize.y), keyPressSize, color);
    }
}

const std::map<Item::itemSlot, std::shared_ptr<Item>> &Character::getItems() const {
    return items;
}

int Character::getHP() const {
    return hp;
}

int Character::getMaxHP() const {
    return maxHP;
}

int Character::getShield() const {
    return shield;
}

int Character::getIncomingPoison() const {
    return incomingPoison;
}

int Character::getIncomingRegen() const {
    return incomingRegen;
}

int Character::getPoison() const {
    return poison;
}

int Character::getRegen() const {
    return regen;
}

bool Character::isBackRow() const {
    return backRow;
}

void Character::addHP(int dHP) {
    hp += dHP;
    hp = std::max(hp, maxHP);
}

}
