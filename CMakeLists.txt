cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(DiceGoneRogue)


set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wnone")
set(CMAKE_CXX_FLAGS "-lstdc++fs" )

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
        src/stb_image.h

        src/utilities/Window.cpp src/utilities/Window.h
        src/shaders/Shader.cpp src/shaders/Shader.h
        src/GameController.cpp src/GameController.h
        src/utilities/InputHandler.cpp src/utilities/InputHandler.h
        src/gameobject/GameObject.cpp src/gameobject/GameObject.h
        src/shaders/Texture2D.cpp src/shaders/Texture2D.h
        src/shaders/SpriteRenderer.cpp src/shaders/SpriteRenderer.h
        src/gameobject/Hero.cpp src/gameobject/Hero.h
        src/gameobject/dice/Dice.cpp src/gameobject/dice/Dice.h
        src/gameobject/dice/Face.cpp src/gameobject/dice/Face.h
        src/iofilemanager/YamlReader.cpp src/iofilemanager/YamlReader.h
        src/utilities/Constants.h
        src/gameobject/dice/FaceModifier.cpp         src/gameobject/dice/FaceModifier.h
        src/gameobject/dice/FaceType.cpp        src/gameobject/dice/FaceType.h
        src/gameobject/Enemy.cpp src/gameobject/Enemy.h
        src/gameobject/Character.cpp src/gameobject/Character.h
        src/utilities/Random.cpp src/utilities/Random.h)

target_link_libraries(DiceGoneRogue
        ${LIBS}
        ${OPENGL_LIBRARY}
        glfw
        ${GLFW_LIBRARIES}

        )
target_link_libraries(DiceGoneRogue stdc++fs)
