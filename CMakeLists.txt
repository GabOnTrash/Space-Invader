cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(Space_Invader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
)

# nlohmann/json
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(raylib nlohmann_json)

add_executable(Space_Invader
        src/AssetsManager.cpp
        src/explosion.cpp
        src/game.cpp
        src/GameLayer.cpp
        src/heart.cpp
        src/laser.cpp
        src/MenuLayer.cpp
        src/meteor.cpp
        src/player.cpp
        src/powerup.cpp
        src/SpaceInvader.cpp
        src/stars.cpp
        UI/Button.cpp
        UI/Label.cpp
        UI/Slider.cpp
)

target_include_directories(Space_Invader PRIVATE
        include
        include/config
        UI
)

target_link_libraries(Space_Invader PRIVATE
        raylib
        nlohmann_json::nlohmann_json
)

add_custom_command(TARGET Space_Invader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Space_Invader>/assets
)

add_custom_command(TARGET Space_Invader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/conf
        $<TARGET_FILE_DIR:Space_Invader>/conf
)
