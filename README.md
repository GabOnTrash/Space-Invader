# Space Invaders 

A modern C++17 implementation of the classic "Space Invaders" arcade game, featuring smooth animations, modifiers, audio effects, and a responsive UI. Built using [Raylib](https://www.raylib.com/) for graphics and audio.

## Features

- **Classic Arcade Gameplay**: Control your spaceship, shoot asteroids, and survive as long as possible.
- **Modifiers**: Collect various modifiers like an extra life, a slowness effect, and more.
- **Difficulty Levels**: Choose from multiple difficulty settings (Easy, Normal, Hard).
- **Audio & Visual Effects**: Includes explosion animations and sound effects.
- **Customizable Controls**: Key bindings and audio settings are saved in a JSON file.
- **Responsive Window**: The game window adapts to your screen resolution.
##
![C++](https://img.shields.io/badge/C%2B%2B-Standard-blue?logo=c%2B%2B&logoColor=white)
![Raylib](https://img.shields.io/badge/Based_on-Raylib-brightgreen?logo=raylib&logoColor=white)
![JSON](https://img.shields.io/badge/Uses-JSON-orange?logo=json&logoColor=white)
![Cmake](https://img.shields.io/badge/Build%20system-cmake-green?logo=cmake&logoColor=white)
![Premake](https://img.shields.io/badge/Build%20system-premake5-purple?logo=lua&logoColor=white)
![Windows](https://img.shields.io/badge/Platform-Windows-blue?logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Platform-Linux-red?logo=linux&logoColor=white)

## Screenshots

![StartMenu](Game/assets/StartMenu.png)
![StartMenu](Game/assets/KeybindsMenu.png)
![StartMenu](Game/assets/AudioMenu.png)
![StartMenu](Game/assets/GamePlay.png)
## Getting Started

### Prerequisites

- C++17 compatible compiler (e.g., MSVC, GCC, Clang)
- [vcpkg](https://github.com/microsoft/vcpkg) for dependency management if you want to
- [Premake5](https://premake.github.io/) for the project generation
- [Cmake](https://cmake.org) project generation and platform indipendency
- [Raylib](https://www.raylib.com/) (install via or cmake, as you like)

### Setup and Build Instructions

## vcpkg and premake5

#### 1. Install Dependencies with vcpkg

Make sure you have [vcpkg](https://github.com/microsoft/vcpkg) installed and integrated with your compiler/IDE.

Install Raylib and any other dependencies: vcpkg install raylib

#### 2. Generate Project Files with Premake5

This project uses [Premake5](https://premake.github.io/) to generate project files.

- Download Premake5 and place the executable in your PATH or project root.
- In the project root, run: premake5 vs2022

This will generate a Visual Studio 2022 solution. For other IDEs or build systems, use the appropriate Premake5 action (e.g., `premake5 gmake2` for GNU Makefiles).

#### 3. Build and Run

- Open the generated solution/project in your IDE (e.g., Visual Studio 2022).
- Build the project.
- Run the executable.

## cmake

#### 1. Create build directory and generate the project

Create in the root the `build` directory move there and type *cmake ..* and choose the toolset (e.g. *-G "Visual Studio 17 2022" -A x64* or *-G "MinGW Makefiles"* for MinGW) 

#### 2. Compile the game

The project will be compiled based on the platform chosed (e.g. with MinGW you should use `mingw32-make`) and the executable alongside the Game/assets folder and the configuration folder will be placed in the current directory


## Project Structure

- Source code is in the main directory.
- Asset files (images, sounds, music) should be placed in the `Game/assets/` directory as referenced in the code.
- configuration files (e.g. `Game/assets.json` & `SpaceInvaderSettings.json`) are located in the `conf/` directory.

## Controls

| Action      | Default Key |
|-------------|-------------|
| Move Up     | W           |
| Move Down   | S           |
| Move Left   | A           |
| Move Right  | D           |
| Shoot       | Space       |

Controls can be customized in the settings menu.

## Modifiers

- **triple_laser**: Triple shot for a limited time.
- **big_laser**: Powerful continuous laser.
- **plus_one_heart**: Gain an extra heart (life).
- **minus_one_heart**: Lose a heart (can die to that).
- **reduced_vel**: Temporary speed reduction.

## Settings

- Audio and control settings are saved in `SpaceInvaderSettings.json`.
- Adjust music, sound effects, and difficulty from the in-game menu.

## Game/assets

- Place all images in the `Game/assets/` directory as referenced in the code.
- Explosion images: `Game/assets/textures/explosion_frames/0.png` to `Game/assets/textures/explosion_frames/20.png`
- Music: `Game/assets/music/game_music.mp3`
- Sound: `Game/assets/sounds/explosion.wav`
- Cursor: `Game/assets/textures/cursors/cursor.png`

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

*Made with Raylib, vcpkg, Premake5, Cmake, and C++17.*
