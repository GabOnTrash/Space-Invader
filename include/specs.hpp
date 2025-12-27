#pragma once

#include <atomic>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <string>
#include <thread>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "AssetsManager.hpp"
#include "Menu.h"
#include "MyRandom.hpp"
#include "timer.hpp"
#include "config/ByteMask.hpp"
#include "config/enums.hpp"
#include "config/keys.hpp"
#include "config/Strings.hpp"

namespace ViewPort
{
	extern int BASE_WIDTH;
	extern int BASE_HEIGHT;
	extern float scale;
	extern float offsetX;
	extern float offsetY;
	extern bool loadedResources;
}

extern KeyBindings KeyBinds;

#define PATH_BYTEMASK_PLAYER "assets/textures/space_ships/Spaceship_5.png"
#define PATH_BYTEMASK_ASTEROID "assets/textures/elements/meteor.png"
#define PATH_BYTEMASK_LASER "assets/textures/elements/laser.png"
#define PATH_BYTEMASK_BIGLASER "assets/textures/elements/big_laser.png"
#define PATH_BYTEMASK_MODIFIERS "assets/textures/modifiers/plus_one_heart.png"
#define PATH_SPACEINVADERS_SETTINGS "conf/settings.json"
#define PATH_ASSETS_CONFIG "conf/assets.json"