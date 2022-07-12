//
// Created by thijs on 07-06-22.
//

#ifndef DICEGONEROGUE_GAMESTATEMANAGER_H
#define DICEGONEROGUE_GAMESTATEMANAGER_H

#include <vector>
#include <utilities/AttackOrder.h>

#include "ui/Window.h"
#include "ui/OnScreenMessage.h"
#include "gameobject/spell/Spell.h"

namespace DGR {

class Scene;

class GameStateManager {
private:
    double dt{};
    double t = 0.0;
    double tPrev = 0.0;

    TextRenderer* textRenderer;
    SpriteRenderer* spriteRenderer;

    Window* window;

    std::vector<OnScreenMessage*> onScreenMessages;

    std::vector<Character*> allHeroes;
    std::vector<Character*> allEnemies;
    std::vector<Scene*> allScenes;

    std::vector<Character*> ownedHeroes;
    std::vector<Scene*> sceneStack;

public:
    explicit GameStateManager(Window* window);

    /// getters
    [[nodiscard]] Window* getWindow() const;

    [[nodiscard]] const std::vector<Scene*> &getAllScenes() const;

    [[nodiscard]] Scene* getScene(const std::string &sceneName, bool onlySceneStack = false) const;

    [[nodiscard]] const std::vector<Scene*> &getSceneStack() const;

    [[nodiscard]] const std::vector<Character*> &getAllHeroes() const;

    [[nodiscard]] const std::vector<Character*> &getAllEnemies() const;

    /// setters
    bool addSceneToStack(const std::string &sceneName, bool disableOtherScenes = true);

    void addOnScreenMessage(OnScreenMessage* message);

    bool popSceneFromStack(bool enableLastSceneInStack = true);

    /// functions
    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void update();

    /// render
    void render();

    void addOnScreenMessage(const std::string &message);
};

}

#endif //DICEGONEROGUE_GAMESTATEMANAGER_H
