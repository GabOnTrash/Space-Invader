#pragma once

#include "enums.hpp"
#include "keys.hpp"
#include "../Renderer/Renderer.hpp"
#include <memory>

struct GameContext
{
    std::shared_ptr<GameState> gameStatus;
    KeyBindings& keyBindings;
    Renderer& renderer;
};

#define PATH_BYTEMASK_PLAYER "assets/textures/space_ships/Spaceship_5.png"
#define PATH_BYTEMASK_ASTEROID "assets/textures/elements/meteor.png"
#define PATH_BYTEMASK_LASER "assets/textures/elements/laser.png"
#define PATH_BYTEMASK_BIGLASER "assets/textures/elements/big_laser.png"
#define PATH_BYTEMASK_MODIFIERS "assets/textures/modifiers/plus_one_heart.png"
#define PATH_SPACEINVADER_SETTINGS "conf/settings.json"
#define PATH_ASSETS_CONFIG "conf/assets.json"
