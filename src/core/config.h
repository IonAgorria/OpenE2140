//
// Created by Ion Agorria on 26/03/18
//
#ifndef OPENE2140_CONFIG_H
#define OPENE2140_CONFIG_H

//Source https://stackoverflow.com/questions/195975/how-to-make-a-char-string-from-a-c-macros-value
#define QUOTE(val) #val
#define STR(val) QUOTE(val)

//Macro for expanding begin end
#define BEGIN_END(container) (container).begin(), (container).end()

//Game related data
#define GAME_NAME "OpenE2140"
#define GAME_VERSION_MAJOR 0
#define GAME_VERSION_MINOR 1
#define GAME_VERSION STR(GAME_VERSION_MAJOR) "." STR(GAME_VERSION_MINOR)
#define GAME_DUMP_FILE "dump.bin"
#define GAME_LOG_FILE "log.txt"
#define GAME_TITLE GAME_NAME " " GAME_VERSION
#define MAIN_LOG "Main"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

#endif //OPENE2140_CONFIG_H
