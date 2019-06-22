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
#ifndef OPENE2140_IS_MACOS
#define OPENE2140_IS_MACOS true
#endif

/* True if being building on windows */
#ifndef OPENE2140_IS_WINDOWS
#define OPENE2140_IS_WINDOWS true
#endif

/* Enables the use of Boost library */
#ifndef OPENE2140_USE_BOOST
#define OPENE2140_USE_BOOST true
#endif

#endif //OPENE2140_BUILD_CONFIG_PLACEHOLDER_H
