cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(DiceGoneRogue)


set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wnone")
set(CMAKE_CXX_FLAGS "-lstdc++fs")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

include_directories(src)

add_subdirectory(include)
include_directories(include
        include/glfw-3.3.7/include/
        include/glm-0.9.7.1/)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

set(INCLUDE_GAMEOBJECT
        src/gameobject/dice/Dice.cpp
        src/gameobject/dice/Dice.h
        src/gameobject/dice/Face.cpp
        src/gameobject/dice/Face.h
        src/gameobject/dice/FaceModifier.cpp
        src/gameobject/dice/FaceModifier.h
        src/gameobject/dice/FaceType.cpp
        src/gameobject/dice/FaceType.h

        src/gameobject/spell/Spell.cpp
        src/gameobject/spell/Spell.h
        src/gameobject/spell/SpellType.cpp
        src/gameobject/spell/SpellType.h

        src/gameobject/Character.cpp
        src/gameobject/Character.h
        src/gameobject/GameObject.cpp
        src/gameobject/GameObject.h
        )

set(INCLUDE_IOFILEMANAGER
        src/io/characters/YamlReader.cpp
        src/io/characters/YamlReader.h
        )

set(INCLUDE_SCENE
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/scene/BattleScene.cpp
        src/scene/BattleScene.h
        src/scene/MainMenuScene.cpp
        src/scene/MainMenuScene.h
        src/scene/SettingsScene.cpp
        src/scene/SettingsScene.h
        src/scene/BattleDefeatScene.cpp
        src/scene/BattleDefeatScene.h
        src/scene/BattleVictoryScene.cpp
        src/scene/BattleVictoryScene.h
        src/scene/CharacterSelectScene.cpp
        src/scene/CharacterSelectScene.h
        )

set(INCLUDE_SHADERS
        src/shaders/Texture2D.cpp
        src/shaders/Texture2D.h
        src/shaders/SpriteRenderer.cpp
        src/shaders/SpriteRenderer.h
        src/shaders/Shader.cpp
        src/shaders/Shader.h
        src/shaders/TextRenderer.cpp
        src/shaders/TextRenderer.h
        )

set(INCLUDE_UI
        src/ui/InputHandler.cpp
        src/ui/InputHandler.h
        src/ui/Button.cpp
        src/ui/Button.h
        src/ui/Window.cpp
        src/ui/Window.h
        src/ui/UIElement.cpp
        src/ui/UIElement.h
        src/ui/OnScreenMessage.cpp src/ui/OnScreenMessage.h)

set(INCLUDE_UTILITIES
        src/utilities/Utilities.cpp
        src/utilities/Utilities.h
        src/utilities/BattleLog.cpp
        src/utilities/BattleLog.h
        src/utilities/Random.cpp
        src/utilities/Random.h

        src/utilities/BiDirectionalMap.h
        src/utilities/Constants.h
        src/scene/LoadGameScene.cpp src/scene/LoadGameScene.h)


add_executable(DiceGoneRogue main.cpp
        src/glad.c
        src/stb_image.h

        ${INCLUDE_GAMEOBJECT}
        ${INCLUDE_IOFILEMANAGER}
        ${INCLUDE_SCENE}
        ${INCLUDE_SHADERS}
        ${INCLUDE_UI}
        ${INCLUDE_UTILITIES}

        src/GameStateManager.cpp
        src/GameStateManager.h
        )

target_link_libraries(DiceGoneRogue
        ${LIBS}
        ${OPENGL_LIBRARY}
        glfw
        ${GLFW_LIBRARIES}
        )

target_link_libraries(DiceGoneRogue stdc++fs)
