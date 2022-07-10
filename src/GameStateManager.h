//
// Created by thijs on 07-06-22.
//

#ifndef DICEGONEROGUE_GAMESTATEMANAGER_H
#define DICEGONEROGUE_GAMESTATEMANAGER_H

#include <memory>
#include <vector>
#include <utilities/BattleLog.h>

#include "ui/Window.h"
#include "ui/OnScreenMessage.h"
#include "gameobject/spell/Spell.h"

namespace DGR {

class Scene;

class GameStateManager : public std::enable_shared_from_this<GameStateManager> {
private:
    double dt{};
    double t = 0.0;
    double tPrev = 0.0;

    std::shared_ptr<TextRenderer> textRenderer;
    std::shared_ptr<SpriteRenderer> spriteRenderer;

    std::shared_ptr<Window> window;

    std::vector<std::shared_ptr<OnScreenMessage>> onScreenMessages;

    std::vector<std::shared_ptr<Character>> allHeroes;
    std::vector<std::shared_ptr<Character>> allEnemies;
    std::vector<std::shared_ptr<Character>> ownedHeroes;
    std::shared_ptr<Character> nullCharacter = nullptr;

    std::vector<std::shared_ptr<Scene>> allScenes;
    std::vector<std::shared_ptr<Scene>> sceneStack;

public:
    explicit GameStateManager(const std::shared_ptr<Window>& window);

    /// getters
    [[nodiscard]] std::shared_ptr<GameStateManager> getSharedFromThis();

    [[nodiscard]] std::shared_ptr<Window> getWindow() const;

    [[nodiscard]] const std::vector<std::shared_ptr<Scene>> &getAllScenes() const;

    [[nodiscard]] std::shared_ptr<Scene> getScene(const std::string &sceneName, bool onlySceneStack = false) const;

    [[nodiscard]] const std::vector<std::shared_ptr<Scene>> &getSceneStack() const;

    [[nodiscard]] const std::vector<std::shared_ptr<Character>> &getAllHeroes() const;

    [[nodiscard]] const std::vector<std::shared_ptr<Character>> &getAllEnemies() const;

    [[nodiscard]] const std::shared_ptr<Character> &getCharacterByID(int id) const;

    /// setters
    bool addSceneToStack(const std::string &sceneName, bool disableOtherScenes = true);

    void addOnScreenMessage(std::shared_ptr<OnScreenMessage> message);

    bool popSceneFromStack(bool enableLastSceneInStack = true);

    /// functions
    void initializeScenes();

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void update();

    /// render
    void render();

    void addOnScreenMessage(const std::string &message);

};

}

#endif //DICEGONEROGUE_GAMESTATEMANAGER_H
