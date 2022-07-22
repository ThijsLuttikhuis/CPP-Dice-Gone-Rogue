//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_GAMEOBJECT_H
#define DICEGONEROGUE_GAMEOBJECT_H

#include <utility>
#include <glm/vec3.hpp>

#include "shaders/TextRenderer.h"
#include "shaders/SpriteRenderer.h"

namespace DGR {

class GameObject {
private:
    static int uniqueIDCounter;
    int uniqueID;

protected:
    std::string name;

    glm::vec2 position{};
    glm::vec2 size{};

    bool hover = false;
public:
    GameObject() {
        uniqueID = uniqueIDCounter++;
    }

    explicit GameObject(std::string name) : name(std::move(name)) {
        uniqueID = uniqueIDCounter++;
    };

    GameObject(std::string name, glm::vec2 position, glm::vec2 size)
          : name(std::move(name)), position(position), size(size) {
        uniqueID = uniqueIDCounter++;
    }

    virtual ~GameObject() = default;

    bool operator ==(const GameObject &other) {
        return uniqueID == other.uniqueID;
    }

    /// getters
    [[nodiscard]] int getUniqueID() const;

    [[nodiscard]] const glm::vec2 &getPosition() const;

    [[nodiscard]] const glm::vec2 &getSize() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] virtual bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] bool getHoverMouse() const;

    /// setters
    void setUniqueID(int uniqueID_);

    void setPosition(glm::vec2 position_);

    void setPosition(int left, int up);

    void setSize(glm::vec2 size);

    void setHoverMouse(bool hover);

    /// render
    virtual void draw(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                      const std::shared_ptr<TextRenderer> &textRenderer) const;

};

}

#endif //DICEGONEROGUE_GAMEOBJECT_H
