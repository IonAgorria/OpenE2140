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
using window_id_t = uint32_t;

/** Entity ID */
using entity_id_t = uint64_t;

/** Tile index */
using tile_index_t = uint32_t;

/** Entity type */
struct entity_type_t {
    /** Kind of this entity */
    uint32_t kind = 0;
    /** Unique id inside the current kind domain */
    uint32_t id = 0;
};

/** Entity direction */
using entity_direction_t = uint32_t;

/** Player ID */
using player_id_t = uint64_t;

/** Money type */
using money_t = int64_t;

/**
 * Type for tile flags
 */
using tile_flags_t = uint32_t;

/**
 * Size struct with shorts
 */
struct size_16_t {
    uint16_t width = 0;
    uint16_t height = 0;
};

/**
 * Struct for key input data
 */
struct input_key_t {
    uint32_t code = 0;
    bool press = false;
    bool repeat = false;
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    bool meta = false;
};

#endif //OPENE2140_TYPES_H
