cmake_minimum_required(VERSION 3.15)
project(TowerStacker LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLAD
add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

add_executable(TowerStacker src/main.cpp src/Utils.cpp src/Shader.cpp src/Renderer.cpp src/Camera.cpp src/Block.cpp src/InputHandler.cpp src/Game.cpp src/TextRenderer.cpp)
target_include_directories(TowerStacker PRIVATE include)
target_link_libraries(TowerStacker PRIVATE glfw glad glm::glm)

if(WIN32)
    target_link_libraries(TowerStacker PRIVATE opengl32)
endif()

install(TARGETS TowerStacker RUNTIME DESTINATION bin)
install(DIRECTORY assets/ DESTINATION bin/assets)
