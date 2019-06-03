//
// Created by Ion Agorria on 2/12/18
//
#ifndef OPENE2140_TYPES_H
#define OPENE2140_TYPES_H

#include <stdint.h>
/** Byte type */
using byte_t = uint8_t;
using byte_array_t = byte_t[];

#include <string>
/** Asset path */
using asset_path_t = std::string;

/** Window ID */
using window_id_t = unsigned int;

/** Entity ID */
using entity_id_t = unsigned long;

/** Entity type */
struct entity_type_t {
    unsigned int kind = 0;
    unsigned int id = 0;
};

/** Entity direction */
using entity_direction_t = unsigned int;

/** Player ID */
using player_id_t = unsigned int;

/** Player ID */
using team_id_t = unsigned int;

/** Money type */
using money_t = signed long;

/**
 * Size struct with shorts
 */
struct size_16_t {
    unsigned short width = 0;
    unsigned short height = 0;
};

/**
 * Struct for key input data
 */
struct input_key_t {
    unsigned int code = 0;
    bool press = false;
    bool repeat = false;
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    bool meta = false;
};

#endif //OPENE2140_TYPES_H
