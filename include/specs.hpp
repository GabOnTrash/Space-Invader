#pragma once

#include <cmath> 
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <stack>
#include <memory>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <raylib.h>
#include <raymath.h>

#include "Menu.h"
#include "timer.hpp"
#include "MyRandom.hpp"
#include "config/Strings.hpp"
#include "config/enums.hpp"
#include "config/keys.hpp"
#include "config/ByteMask.hpp"
#include "AssetsManager.hpp"

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
#define PATH_SPACEINVADERS_SETTINGS "conf/SpaceInvaderSettings.json"
#define PATH_ASSETS_CONFIG "conf/assets.json"