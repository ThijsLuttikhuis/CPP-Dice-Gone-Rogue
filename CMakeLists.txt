cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(DiceGoneRogue)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra -Werror")
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

set(INCLUDE_GAMELOGIC
        src/gamelogic/Inventory.cpp
        src/gamelogic/Inventory.h
        src/gamelogic/GameProgress.cpp
        src/gamelogic/GameProgress.h
        src/gamelogic/BattleController.cpp
        src/gamelogic/BattleController.h)

set(INCLUDE_GAMEOBJECT
        src/gameobject/dice/Dice.cpp
        src/gameobject/dice/Dice.h
        src/gameobject/dice/face/Face.cpp
        src/gameobject/dice/face/Face.h
        src/gameobject/dice/face/FaceModifier.cpp
        src/gameobject/dice/face/FaceModifier.h
        src/gameobject/dice/face/FaceType.cpp
        src/gameobject/dice/face/FaceType.h
        src/gameobject/dice/face/FaceShield.cpp
        src/gameobject/dice/face/FaceShield.h
        src/gameobject/dice/face/FaceEmpty.cpp
        src/gameobject/dice/face/FaceEmpty.h
        src/gameobject/dice/face/FaceHeal.cpp
        src/gameobject/dice/face/FaceHeal.h
        src/gameobject/dice/face/FaceDodge.cpp
        src/gameobject/dice/face/FaceDodge.h
        src/gameobject/dice/face/FaceMana.cpp
        src/gameobject/dice/face/FaceMana.h
        src/gameobject/dice/face/FaceDamage.cpp
        src/gameobject/dice/face/FaceDamage.h
        src/gameobject/dice/face/FaceHealAndShield.cpp
        src/gameobject/dice/face/FaceHealAndShield.h

        src/gameobject/item/Item.cpp
        src/gameobject/item/Item.h
        src/gameobject/item/ItemEffect.cpp
        src/gameobject/item/ItemEffect.h

        src/gameobject/spell/Spell.cpp
        src/gameobject/spell/Spell.h
        src/gameobject/spell/SpellType.cpp
        src/gameobject/spell/SpellType.h

        src/gameobject/passive/PassiveEffect.cpp
        src/gameobject/passive/PassiveEffect.h

        src/gameobject/Character.cpp
        src/gameobject/Character.h
        src/gameobject/GameObject.cpp
        src/gameobject/GameObject.h
        )

set(INCLUDE_IOFILEMANAGER
        src/io/yamlreader/YamlReader.cpp
        src/io/yamlreader/YamlReader.h
        src/io/yamlreader/YamlReaderCharacters.h
        src/io/yamlreader/YamlReaderItems.h

        src/gamelogic/BattleLog.cpp
        src/gamelogic/BattleLog.h
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
        src/scene/LoadGameScene.cpp
        src/scene/LoadGameScene.h
        src/scene/AreYouSureScene.cpp
        src/scene/AreYouSureScene.h
        src/scene/LevelSelectScene.cpp
        src/scene/LevelSelectScene.h
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
        src/ui/OnScreenMessage.cpp
        src/ui/OnScreenMessage.h
        )

set(INCLUDE_UTILITIES
        src/utilities/Utilities.cpp
        src/utilities/Utilities.h
        src/utilities/Random.cpp
        src/utilities/Random.h

        src/utilities/BiDirectionalMap.h
        src/utilities/Constants.h
        )

add_executable(DiceGoneRogue main.cpp
        src/glad.c
        src/stb_image.h

        ${INCLUDE_GAMELOGIC}
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
