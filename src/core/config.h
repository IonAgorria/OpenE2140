//
// Created by Ion Agorria on 26/03/18
//
#ifndef OPENE2140_CONFIG_H
#define OPENE2140_CONFIG_H

//Source https://stackoverflow.com/questions/195975/how-to-make-a-char-string-from-a-c-macros-value
#define QUOTE(val) #val
#define STR(val) QUOTE(val)

/** Macro for expanding begin end */
#define BEGIN_END(container) (container).begin(), (container).end()

/** Check if we are in Windows */
#ifdef _WIN32
#define IS_WINDOWS 1
#endif

/** Dir separator */
#ifdef IS_WINDOWS
#define DIR_SEP "\\"
#else
#define DIR_SEP "/"
#endif

//Game related data
/** Game name */
#define GAME_NAME "OpenE2140"
/** Game version major digit */
#define GAME_VERSION_MAJOR 0
/** Game version minor digit */
#define GAME_VERSION_MINOR 1
/** Game version string */
#define GAME_VERSION STR(GAME_VERSION_MAJOR) "." STR(GAME_VERSION_MINOR)
/** File to dump when crashing */
#define GAME_DUMP_FILE "dump.bin"
/** File to store log output */
#define GAME_LOG_FILE "log.txt"
/** Full title of this game */
#define GAME_TITLE GAME_NAME " " GAME_VERSION
/** Game main log tag for global use when required */
#define MAIN_LOG "Main"
/** Default resolution width */
#define DEFAULT_RESOLUTION_WIDTH 800
/** Default resolution height */
#define DEFAULT_RESOLUTION_HEIGHT 600

#endif //OPENE2140_CONFIG_H
