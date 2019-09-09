# OpenE2140

## About
An open source game reimplementing Earth 2140 gameplay using original game files.

The project aims to reproduce the overall gameplay experience while adding a modern game features
that are not available in original game.

NOTE: The project is still under heavy development and not much happens yet.

## Requirements and libraries
For compiling:
- Meson
- C++17 capable compiler

This project includes these libraries as git submodules or bundled:
- spdlog (git submodule) - Adds log file and console handling and formatting
- stb (git submodule) - Contains various libs
- cnl (git submodule) - Fixed point library
- json (git submodule) - JSON library for reading and writting JSON strings

This project requires to have the following libraries installed:
- LibSDL 2.0 - Handles graphics, input and basic filesystem
- GLEW - Hadles the set up of OpenGL context
- glm - Library containing utilies for OpenGL related operations

This projects uses optional libraries to enhance some functionalities:
- Boost 1.66 or newer - Allows to list directories and generate readable stacktraces in platform independent manner

## Compiling
To download and compile this project:
1. Download the repository using following command to include submodules:
```
git clone --recursive **url**
```

2. Compile according to your platform:
    - Linux:
        Execute `./build-linux.sh`
    - Mac OS:
        Execute `./build-macos.sh`
        
3. Find the generated files:
    - Linux:
        The executable should be called `opene2140`
    - MacOS:
        The executable should be called `opene2140`

## Running
Original game files are required to be located in folder **assets** in the same path as game executable.
WD archive files are the original game format used for storing assets and is the preffered method.
Directories takes precedence over WD files when loading and this method is mainly oriented for development.

The following game assets must be present in WD file or directory form:

| WD file | Directory | About |
| --- | --- | --- |
| MIX.WD | MIX | MIX files containing game sprites |
| PIRO.WD | PIRO | Graphic effects like explosions |
| LEVEL.WD | LEVEL | Level maps and text |

## Completed
- World/map code and drawing of map tiles
- Basic entity implementation
- Custom player colors
- Basic palette setup such as light, player and shadow
- Trees and other map objects
- Config load/save
- Units and buildings stats

## Roadmap
- Use fixed point numbers
- Use delta for updates
- Entity attachment
    - Proper adjustments of turret offsets
    - Building exits
    - Conveyor belt
    - Unit turret
- Order processing
- Unit factory
- Mobile units
- Move order
- Basic pathfinder
- Rectangle and circular collisions for buildings/units 
- Basic ingame GUI
- Menu GUI
- Weapons:
    - Weapon data loading into Weapon(Config?) at simulation startup
    - Usage of weapon data when weapon is instanced
- Turrets:
    - Turret(s) containing component
    - Tracking the current target with turret
- Attack order
- Add ingame menu for viewing player stuff
- Refinery and Power plant smoke
- Building construction and deployment
- Factories build queue
- Writing game data (sprites of units, weapons and other missing data)
- Complex orders such as patrol, ore transport, unit transport
- Minimap to show units, buildings and ore
- Map trees, rocks and water
- Saving/loading
- Basic AI
- Audio from game files (16000hz 8bits)
- Game rebalancing
- Multiplayer

## Mentions
- WD and DAT+PAL decoding is based on tools made in Pascal. Source and compiled exe available at http://www.ctpax-x.org/?goto=files&show=17
- MIX files, levels, sprites and tiles decoding is based on work done by "mnn" at http://www.gamedev.net/topic/582821-sprite-formats-used-in-old-games/