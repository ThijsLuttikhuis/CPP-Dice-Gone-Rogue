//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_SHADER_H
#define DICEGONEROGUE_SHADER_H

#include <memory>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DGR {

class Shader {
public:
    unsigned int id{};

    Shader() = default;

    // sets the current shader as active
    void use();

    // compiles the shader from given source code
    void compile(const std::string &vertexSource, const std::string &fragmentSource,
                 const std::string &geometrySource = "");

    void setFloat(const char* name, float value);

    void setInteger(const char* name, int value);

    void setVector2f(const char* name, float x, float y);

    void setVector2f(const char* name, const glm::vec2 &value);

    void setVector3f(const char* name, float x, float y, float z);

    void setVector3f(const char* name, const glm::vec3 &value);

    void setVector4f(const char* name, float x, float y, float z, float w);

    void setVector4f(const char* name, const glm::vec4 &value);

    void setMatrix4(const char* name, const glm::mat4 &matrix);

private:
    void checkCompileErrors(unsigned int object, const std::string &type);
};

}

#endif //DICEGONEROGUE_SHADER_H
