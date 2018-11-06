# OpenE2140

## About
An open source game reimplementing Earth 2140 gameplay using original game files.
The project is still under heavy development and not much happens yet.

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
        
3. Find the generated files:
    - Linux:
        The main executable should be called `opene2140`

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


## Mentions
- WD and DAT+PAL decoding is based on tools made in Pascal. Source and compiled exe available on http://www.ctpax-x.org/?goto=files&show=17
- MIX files, sprites and tiles decoding is based on work done by "mnn" on http://www.gamedev.net/topic/582821-sprite-formats-used-in-old-games/