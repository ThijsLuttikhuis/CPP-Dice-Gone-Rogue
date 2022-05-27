cmake_minimum_required(VERSION 3.10)

project(DiceGoneRogue)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wnone")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
add_subdirectory(include)
include_directories(include
        include/glfw-3.1.2/include/
        include/glm-0.9.7.1/
)
add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

add_executable(DiceGoneRogue main.cpp src/glad.c)

target_link_libraries(DiceGoneRogue
        ${LIBS}
        ${OPENGL_LIBRARY}
        glfw
        ${GLFW_LIBRARIES}
        )
