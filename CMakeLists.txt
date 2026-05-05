cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(Space-Invader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Opzioni di Build (Entrambe ON di default) ---
option(BUILD_CLIENT "Compile Client (needs Raylib)" ON)
option(BUILD_SERVER "Compile Server" ON)

# --- Dependencies ---
include(FetchContent)

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

FetchContent_MakeAvailable(nlohmann_json asio)
find_package(OpenSSL REQUIRED)

set(NETWORK_SOURCES
        Network/src/Client.cpp
        Network/src/Server.cpp
        Game/GameElements/util/Logger.hpp
)

if (BUILD_CLIENT)

    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)

    set(GAME_SOURCES
        Game/GameElements/AssetsManager/AssetsManager.cpp
        Game/GameElements/CollisionManager/CollisionManager.cpp
        Game/GameElements/Explosion/Explosion.cpp
        Game/GameElements/Game/Game.cpp
        Game/GameElements/Heart/Heart.cpp
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
        Game/UI/Button.cpp
        Game/UI/Slider.cpp
        Game/GameElements/EntitiesManaging/EntityManager.hpp
        Game/UI/TextBox.cpp
        Game/UI/TextBox.hpp
        Game/GameElements/MenuHandle/MenuInitializer.cpp
        Game/GameElements/util/GameContext.hpp
        Game/GameElements/util/performance.hpp
    )

    add_executable(Space-Invader
            Game/Space-Invader.cpp
            ${GAME_SOURCES}
            ${NETWORK_SOURCES}
    )

    set_target_properties(Space-Invader PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Client"
    )

    target_include_directories(Space-Invader PRIVATE
            Game/assets
            Game/conf
            Game
            Game/UI
            Network/include
            ${asio_SOURCE_DIR}/asio/include
            ${OPENSSL_INCLUDE_DIR}
    )

    target_compile_definitions(Space-Invader PUBLIC ASIO_HAS_SSL)

    if (WIN32)
        target_link_libraries(Space-Invader PRIVATE raylib nlohmann_json::nlohmann_json OpenSSL::SSL OpenSSL::Crypto ws2_32)
    else()
        target_link_libraries(Space-Invader PRIVATE raylib nlohmann_json::nlohmann_json OpenSSL::SSL OpenSSL::Crypto pthread)
    endif()

    # --- CLIENT FOLDER ---
    add_custom_command(TARGET Space-Invader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Game/assets $<TARGET_FILE_DIR:Space-Invader>/assets
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Game/conf $<TARGET_FILE_DIR:Space-Invader>/conf
            COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Space-Invader>/auth
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/auth/space_invader_server.crt $<TARGET_FILE_DIR:Space-Invader>/auth/space_invader_server.crt
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/auth/address.txt $<TARGET_FILE_DIR:Space-Invader>/auth/address.txt
            COMMENT "Client folder."
    )
endif()


if (BUILD_SERVER)
    add_executable(Space-Invader-Server
            Server/src/Space-Invader-Server.cpp
            ${NETWORK_SOURCES}
    )

    set_target_properties(Space-Invader-Server PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Server"
    )

    target_include_directories(Space-Invader-Server PRIVATE
            Network/include
            ${asio_SOURCE_DIR}/asio/include
            ${OPENSSL_INCLUDE_DIR}
    )

    target_compile_definitions(Space-Invader-Server PUBLIC ASIO_HAS_SSL)

    if (WIN32)
        target_link_libraries(Space-Invader-Server PRIVATE OpenSSL::SSL OpenSSL::Crypto ws2_32)
    else()
        target_link_libraries(Space-Invader-Server PRIVATE OpenSSL::SSL OpenSSL::Crypto pthread)
    endif()

    # --- SERVER FOLDER ---
    add_custom_command(TARGET Space-Invader-Server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Space-Invader-Server>/auth
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/auth/space_invader_server.crt $<TARGET_FILE_DIR:Space-Invader-Server>/auth/space_invader_server.crt
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/auth/space_invader_server.key $<TARGET_FILE_DIR:Space-Invader-Server>/auth/space_invader_server.key
            COMMENT "Server folder."
    )
endif()