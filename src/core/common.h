//
// Created by Ion Agorria on 26/03/18
//

#include "macros.h"
#include "types.h"
#include "config.h"

#ifndef OPENE2140_COMMON_H
#define OPENE2140_COMMON_H

/** Game version string */
#define GAME_VERSION STR(GAME_VERSION_MAJOR) "." STR(GAME_VERSION_MINOR)
/** Full title of this game */
#define GAME_TITLE GAME_NAME " " GAME_VERSION
/** Game main log tag for global use when required */
#define MAIN_LOG "Main"
/** Minimum texture size */
#define MINIMUM_TEXTURE_SIZE 1024
/** Texture unit for image texture */
#define TEXTURE_UNIT_IMAGE GL_TEXTURE0
/** Texture unit for colors palette texture */
#define TEXTURE_UNIT_PALETTE_COLORS GL_TEXTURE1
/** Texture unit for extra palette texture */
#define TEXTURE_UNIT_PALETTE_EXTRA GL_TEXTURE2

#endif //OPENE2140_COMMON_H
