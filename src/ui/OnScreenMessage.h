//
// Created by thijs on 26-06-22.
//

#ifndef DICEGONEROGUE_ONSCREENMESSAGE_H
#define DICEGONEROGUE_ONSCREENMESSAGE_H


#include <string>
#include <glm/glm.hpp>
#include <utilities/Constants.h>

#include "shaders/SpriteRenderer.h"
#include "shaders/TextRenderer.h"
#include "UIElement.h"

namespace DGR {

class OnScreenMessage : public UIElement {
private:
    std::string text;
    double duration = 1.5;
    float moveSpeed = 20;
    float fadeSpeed = 1.0;

public:
    explicit OnScreenMessage(const std::string &name,
                    const glm::vec2 &pos = glm::vec2(DGR_WIDTH / 2 - 100, DGR_HEIGHT * 0.7),
                    const glm::vec2 &size = glm::vec2(200, 8),
                    const glm::vec3 &color = glm::vec3(0.9f, 0.2f, 0.4f),
                    float alpha = 0.5f)
          : UIElement(name, pos, size, color, alpha), text(name) {}

    [[nodiscard]] double getDuration() const;

    void setFade(float fade_);

    void setDuration(double duration_);

    void setText(std::string text_);

    void update(double dt);

    void draw( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer) const override;

};

}

#endif //DICEGONEROGUE_ONSCREENMESSAGE_H
