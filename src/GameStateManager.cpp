//
// Created by thijs on 07-06-22.
//

#include "GameStateManager.h"

namespace DGR {

GameStateManager::gameState GameStateManager::getGameState() const {
    return state;
}

bool GameStateManager::isHeroesTurn() const {
    return heroesTurn;
}

Character* GameStateManager::getClickedCharacter() const {
    return clickedCharacter;
}

void GameStateManager::setClickedCharacter(Character* clickedCharacter_) {
    clickedCharacter = clickedCharacter_;
}

int GameStateManager::reroll() {
    if (rerolls > 0) {
        rerolls --;
        for (auto &hero : heroes) {
            hero->roll();
        }
    }
#ifdef DEBUG
    else {
        std::cerr << "no rerolls remaining!" << std::endl;
    }
#endif

    return rerolls;
}

const std::vector<Hero*> &GameStateManager::getHeroes() const {
    return heroes;
}

const std::vector<Enemy*> &GameStateManager::getEnemies() const {
    return enemies;
}

void GameStateManager::setHeroes(const std::vector<Hero*> &heroes_) {
    heroes = heroes_;
}

void GameStateManager::setEnemies(const std::vector<Enemy*> &enemies_) {
    enemies = enemies_;
}

void GameStateManager::setGameState(GameStateManager::gameState state_) {
    state = state_;
}

void GameStateManager::render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) {
    if (clickedCharacter) {
        clickedCharacter->drawBox(spriteRenderer, glm::vec3(0.7f, 0.0f, 0.0f));
    }
}

}