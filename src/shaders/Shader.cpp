//
// Created by thijs on 30-05-22.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"

namespace DGR {

void Shader::use() {
    glUseProgram(id);
}

void Shader::compile(const std::string &vertexFile, const std::string &fragmentFile, const std::string &geometryFile) {
    std::string vertexString, fragmentString, geometryString;
    unsigned int vertexShader{}, fragmentShader{}, geometryShader{};

    bool hasGeometry = !geometryFile.empty();

    // get vertex file contents
    std::ifstream vertexIfstream(vertexFile);
    std::stringstream vertexSStream;
    vertexSStream << vertexIfstream.rdbuf();
    vertexIfstream.close();
    vertexString = vertexSStream.str();

    // get fragment file contents
    std::ifstream fragmentIfstream(fragmentFile);
    std::stringstream fragmentSStream;
    fragmentSStream << fragmentIfstream.rdbuf();
    fragmentIfstream.close();
    fragmentString = fragmentSStream.str();

    // get geometry file contents
    if (hasGeometry) {
        std::ifstream geometryIfstream(fragmentFile);
        std::stringstream geometrySStream;
        geometrySStream << geometryIfstream.rdbuf();
        geometryIfstream.close();
        geometryString = geometrySStream.str();
    }

    // create vertex shader
    const char* vertexSource = vertexString.c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // create fragment shader
    const char* fragmentSource = fragmentString.c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // create geometry shader if exists
    if (hasGeometry) {
        const char* geometrySource = geometryString.c_str();
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometrySource, nullptr);
        glCompileShader(geometryShader);
        checkCompileErrors(geometryShader, "GEOMETRY");
    }

    // create program and attach shaders
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    if (hasGeometry) {
        glAttachShader(id, geometryShader);
    }

    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (hasGeometry) {
        glDeleteShader(geometryShader);
    }
}

void Shader::setFloat(const char* name, float value) {
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setInteger(const char* name, int value) {
    glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::setVector2f(const char* name, float x, float y) {
    glUniform2f(glGetUniformLocation(id, name), x, y);
}

void Shader::setVector2f(const char* name, const glm::vec2 &value) {
    glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}

void Shader::setVector3f(const char* name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(id, name), x, y, z);
}

void Shader::setVector3f(const char* name, const glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const char* name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}

void Shader::setVector4f(const char* name, const glm::vec4 &value) {
    glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const char* name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned int object, const std::string &type) {
    int success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
    }

    if (!success) {
        glGetShaderInfoLog(object, 1024, nullptr, infoLog);
        std::cout << "Shader::compile error: type: " << type << ", id: " << id << ", log: " << infoLog << std::endl;
        exit(-1);
    }
}

}
