cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(Space-Invader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG master
)

FetchContent_MakeAvailable(raylib nlohmann_json asio)

# --- 1. Network Library (Shared by Client and Server) ---
# Contains only Network logic, no Game logic.
add_library(ClientServerImplementation STATIC
        Network/src/Client.cpp
        Network/src/Server.cpp
)

target_include_directories(ClientServerImplementation PUBLIC
        Network/include
        ${asio_SOURCE_DIR}/asio/include
)

target_compile_definitions(ClientServerImplementation PUBLIC ASIO_STANDALONE)

if (WIN32)
    target_link_libraries(ClientServerImplementation PUBLIC ws2_32)
elseif(UNIX)
    target_link_libraries(ClientServerImplementation PUBLIC pthread)
endif()

# --- 2. Space-Invader Client (The Game) ---

# Define the source files for the game.
# Adjusted paths assuming "Game/Name/Name.cpp" structure.
set(GAME_SOURCES
        Game/GameElements/AssetsManager/AssetsManager.cpp
        Game/GameElements/AudioManager/AudioManager.cpp
        Game/GameElements/CollisionManager/CollisionManager.cpp
        Game/GameElements/Explosion/Explosion.cpp
        Game/GameElements/Game/Game.cpp
        Game/GameElements/Heart/Heart.cpp
        Game/GameElements/IGameMode/IGameMode.cpp
        Game/GameElements/Laser/Laser.cpp
        Game/GameElements/MenuHandle/MenuHandle.cpp
        Game/GameElements/Meteor/Meteor.cpp
        Game/GameElements/Modifier/Modifier.cpp
        Game/GameElements/MultiPlayerMode/MultiPlayerMode.cpp
        Game/GameElements/Player/Player.cpp
        Game/GameElements/Renderer/Renderer.cpp
        Game/GameElements/SettingsManager/SettingsManager.cpp
        Game/GameElements/SinglePlayerMode/SinglePlayerMode.cpp
        Game/GameElements/Stars/Stars.cpp
        # UI Elements
        Game/UI/Button.cpp
        Game/UI/Label.cpp
        Game/UI/Slider.cpp
        # Managers (Entities)
        Game/GameElements/EntitiesManaging/EntityManager.hpp # Headers are usually optional in add_executable but good for IDEs
)

# Main Executable
add_executable(Space-Invader
        Game/Space-Invader.cpp   # The main entry point
        ${GAME_SOURCES}
)

# Include directories so you can do #include "Player/Player.hpp" or #include "Game/Game.hpp"
target_include_directories(Space-Invader PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Game
        ${CMAKE_CURRENT_SOURCE_DIR}/Game/UI
        ${CMAKE_CURRENT_SOURCE_DIR}/Network/include
)

target_link_libraries(Space-Invader PRIVATE
        raylib
        nlohmann_json::nlohmann_json
        ClientServerImplementation
)

# --- 3. Space-Invader Server ---

add_executable(Space-Invader-Server
        Server/src/Space-Invader-Server.cpp
)

target_include_directories(Space-Invader-Server PRIVATE
        Network/include
        ${asio_SOURCE_DIR}/asio/include
)

# Link against the shared network library
target_link_libraries(Space-Invader-Server PRIVATE
        ClientServerImplementation
)

# --- Post-Build Steps (Copy Assets) ---

# Output directory for binaries
set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

# Copy Assets folder
add_custom_command(TARGET Space-Invader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Game/assets
        ${OUTPUT_DIR}/assets
        COMMENT "Copying Assets..."
)

# Copy Config/Conf folder
add_custom_command(TARGET Space-Invader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Game/conf
        ${OUTPUT_DIR}/conf
        COMMENT "Copying Configs..."
)