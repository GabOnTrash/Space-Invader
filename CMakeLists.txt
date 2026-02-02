cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(Space-Invader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

set_target_properties(ClientServerImplementation PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}  # static lib
)

add_executable(Space-Invader
    Game/src/AssetsManager.cpp
    Game/src/explosion.cpp
    Game/src/game.cpp
    Game/src/GameLayer.cpp
    Game/src/heart.cpp
    Game/src/laser.cpp
    Game/src/MenuLayer.cpp
    Game/src/meteor.cpp
    Game/src/player.cpp
    Game/src/modifier.cpp
    Game/src/SpaceInvader.cpp
    Game/src/stars.cpp
    Game/UI/Button.cpp
    Game/UI/Label.cpp
    Game/UI/Slider.cpp
)

target_include_directories(Space-Invader PRIVATE
    Game/include
    Game/include/config
    Game/UI
)

target_link_libraries(Space-Invader PRIVATE
    raylib
    nlohmann_json::nlohmann_json
    ClientServerImplementation
)

set_target_properties(Space-Invader PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)


add_executable(Space-Invader-Server
        Server/src/Space-Invader-Server.cpp
    Network/src/Server.cpp
)

target_include_directories(Space-Invader-Server PRIVATE
    Network/include
    ${asio_SOURCE_DIR}/asio/include
)

target_link_libraries(Space-Invader-Server PRIVATE
    ClientServerImplementation
)

set_target_properties(Space-Invader-Server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

add_custom_command(TARGET Space-Invader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Game/assets
    ${OUTPUT_DIR}/assets
)

add_custom_command(TARGET Space-Invader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Game/conf
    ${OUTPUT_DIR}/conf
)

add_custom_command(TARGET Space-Invader-Server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Space-Invader-Server> ${OUTPUT_DIR}/$<TARGET_FILE_NAME:Space-Invader-Server>
)
