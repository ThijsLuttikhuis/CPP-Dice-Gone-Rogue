cmake_minimum_required(VERSION 3.11)

project(DiceGoneRogue)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wnone")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")



include_directories(src)

add_subdirectory(include)
include_directories(include
        include/glfw-3.3.7/include/
        include/glm-0.9.7.1/)

find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

add_executable(DiceGoneRogue main.cpp
        src/glad.c
        src/Window.cpp src/Window.h
        src/Shader.cpp src/Shader.h
        src/GameController.cpp src/GameController.h
        src/InputHandler.cpp src/InputHandler.h src/gameobject/GameObject.cpp src/gameobject/GameObject.h src/Texture2D.cpp src/Texture2D.h src/stb_image.h src/SpriteRenderer.cpp src/SpriteRenderer.h src/gameobject/Hero.cpp src/gameobject/Hero.h)

target_link_libraries(DiceGoneRogue
        ${LIBS}
        ${OPENGL_LIBRARY}
        glfw
        ${GLFW_LIBRARIES}
        )
