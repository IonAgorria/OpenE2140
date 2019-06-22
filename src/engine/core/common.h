//
// Created by Ion Agorria on 26/03/18
//

#ifndef OPENE2140_COMMON_H
#define OPENE2140_COMMON_H

//Include GL in proper order
#include <GL/glew.h>
#include <SDL_opengl.h>

//Includes
#include "macros.h"
#include "types.h"
#include "build_config.h"
#include "build_config_placeholder.h"

//Includes for common stuff
#include <memory>
#include <vector>
#include <string>

//Constants
/** Game version string */
#define GAME_VERSION STR(GAME_VERSION_MAJOR) "." STR(GAME_VERSION_MINOR)
/** Full title of this game */
#define GAME_TITLE GAME_NAME " " GAME_VERSION
/** Game main log tag for global use when required */
#define MAIN_LOG "Main"
/** File to dump when crashing */
#define GAME_DUMP_FILE "dump.bin"
/** File to store log output */
#define GAME_LOG_FILE "log.txt"
/** Default window resolution */
#define WINDOW_DEFAULT_RESOLUTION 1024, 768
/** Directory name containing assets */
#define GAME_ASSETS_DIR "assets"
/** Minimum texture size (texture packer fails under this) */
#define MINIMUM_TEXTURE_SIZE 512
/** Texture unit for palette image texture */
#define TEXTURE_UNIT_IMAGE_PALETTE GL_TEXTURE0
/** Texture unit for RGBA image texture */
#define TEXTURE_UNIT_IMAGE_RGBA GL_TEXTURE1
/** Texture unit for colors palette texture */
#define TEXTURE_UNIT_PALETTE_COLORS GL_TEXTURE2
/** Texture unit for extra palette texture */
#define TEXTURE_UNIT_PALETTE_EXTRA GL_TEXTURE3
/** Constant for infinity cost */
#define PATHFINDER_INFINITY (static_cast<path_cost_t>(-1))
/** Flags for tile states */
#define TILE_FLAG_PASSABLE               0b1
#define TILE_FLAG_WATER                 0b10
#define TILE_FLAG_SHORE                0b100
#define TILE_FLAG_IMMUTABLE           0b1000 //Flag for immutable (is not affected by explosions or other stuff
#define TILE_FLAG_SAND               0b10000
#define TILE_FLAG_ENTITY_TERRAIN    0b100000 //Flag if contains an entity in ground/water
#define TILE_FLAG_ENTITY_AIR       0b1000000 //Flag if contains entity in air

#endif //OPENE2140_COMMON_H
