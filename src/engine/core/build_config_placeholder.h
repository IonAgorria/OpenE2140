//
// Created by Ion Agorria on 03/01/19
//

/**
 * This only defines the data type for configs so the IDE doesn't complain about missing defs
 */

#ifndef OPENE2140_BUILD_CONFIG_PLACEHOLDER_H
#define OPENE2140_BUILD_CONFIG_PLACEHOLDER_H

/* Name */
#ifndef GAME_NAME
#define GAME_NAME ""
#endif

/* Version major digit */
#ifndef GAME_VERSION_MAJOR
#define GAME_VERSION_MAJOR 0
#endif

/* Version minor digit */
#ifndef GAME_VERSION_MINOR
#define GAME_VERSION_MINOR 0
#endif

/* True if being building on macos */
#ifndef GAME_IS_MACOS
#define GAME_IS_MACOS true
#endif

/* True if being building on windows */
#ifndef GAME_IS_WINDOWS
#define GAME_IS_WINDOWS true
#endif

/* Enables the use of Boost library */
#ifndef GAME_USE_BOOST
#define GAME_USE_BOOST true
#endif

/* Updates of game logic per second */
#ifndef GAME_UPDATES_PER_SECOND
#define GAME_UPDATES_PER_SECOND 30
#endif

#endif //OPENE2140_BUILD_CONFIG_PLACEHOLDER_H
