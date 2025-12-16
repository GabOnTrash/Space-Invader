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

#include "timer.hpp"
#include "MyRandom.hpp"
#include "Menu.h"
#include "config/Strings.hpp"
#include "config/enums.hpp"
#include "config/keys.hpp"
#include "config/ByteMask.hpp"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern float SCALE;
extern KeyBindings KeyBinds;