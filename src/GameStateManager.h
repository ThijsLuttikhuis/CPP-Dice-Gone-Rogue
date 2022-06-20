//
// Created by thijs on 07-06-22.
//

#ifndef DICEGONEROGUE_GAMESTATEMANAGER_H
#define DICEGONEROGUE_GAMESTATEMANAGER_H

#include <vector>
#include <utilities/AttackOrder.h>

#include "ui/Window.h"
#include "gameobject/spell/Spell.h"

namespace DGR {

class Scene;

class GameStateManager {
private:
    double dt{};
    double t{};
    double tPrev = 0.0;

    TextRenderer* textRenderer;
    SpriteRenderer* spriteRenderer;

    Window* window;
    std::vector<Scene*> allScenes;

    std::vector<Scene*> sceneStack;

public:
    explicit GameStateManager(Window* window);

    /// getters
    [[nodiscard]] Window* getWindow() const;

    [[nodiscard]] const std::vector<Scene*> &getAllScenes() const;

    [[nodiscard]] const std::vector<Scene*> &getSceneStack() const;

    /// setters


    /// functions
    bool addSceneToStack(const std::string &sceneName, bool disableOtherScenes = true);

    bool popSceneFromStack(bool enableLastSceneInStack = true);

    void handleMouseButton(double xPos, double yPos);

    void handleMousePosition(double xPos, double yPos);

    void update();

    /// render
    void render();

};

}

#endif //DICEGONEROGUE_GAMESTATEMANAGER_H
