# OpenE2140

## About
An open source game reimplementing Earth 2140 gameplay using original game files.

The project aims to reproduce the overall gameplay experience while adding a modern game features
that are not available in original game.

NOTE: The project is still under heavy development and not much happens yet.

## Libraries
This project includes these libraries as git submodules or bundled:
- spdlog (git submodule) - Adds log file and console handling and formatting
- stb (git submodule) - Contains various libs

This project requires to have the following libraries installed:
- LibSDL 2.0 - Handles graphics, input and basic filesystem

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

## Roadmap
- Writting game data
- Basic entity implementation
- Entity drawing
- Static map entities (using SPRO*)
- Entity selection
- Mobile units
- Move order
- Basic pathfinder
- Buildings
- Rectangle and circular collisions for buildings/units 
- Basic ingame GUI
- Menu GUI
- Weapons
- Turrets
- Attack order
- Add ingame menu for viewing player stuff
- Building construction and deployment
- Factories build queue
- Player colors/teams
- Complex orders such as patrol, ore transport, unit transport
- Minimap to show units, buildings and ore
- Map trees, rocks and water
- Saving/loading
- Basic AI
- Audio from game files (16000hz 8bits)
- Game rebalancing
- Multiplayer

## Mentions
- WD and DAT+PAL decoding is based on tools made in Pascal. Source and compiled exe available on http://www.ctpax-x.org/?goto=files&show=17
- MIX files, levels, sprites and tiles decoding is based on work done by "mnn" on http://www.gamedev.net/topic/582821-sprite-formats-used-in-old-games/