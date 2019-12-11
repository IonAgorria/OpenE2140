//
// Created by Ion Agorria on 2/12/18
//
#ifndef OPENE2140_TYPES_H
#define OPENE2140_TYPES_H

#include <cstdint>
/** Byte type */
using byte_t = uint8_t;
using byte_array_t = byte_t[];

#include <string>
/** Asset path */
using asset_path_t = std::string;

/** Config stuff */
#include <nlohmann/json.hpp>
using config_data_t = nlohmann::json;
using config_key_t = nlohmann::json::object_t::key_type;

/** Window ID */
using window_id_t = uint32_t;

/** Entity ID */
using entity_id_t = uint64_t;

/** Faction ID */
using faction_id_t = uint16_t;

/** Tile index */
using tile_index_t = uint16_t;

/** Entity type kind id */
using entity_kind_t = uint32_t;

/** Unique id inside the kind domain */
using entity_type_id_t = uint32_t;

/** Entity type id (kind + type id or code inside kind) */
struct entity_type_t {
    entity_kind_t kind = 0;
    entity_type_id_t id = 0;
};

/** Entity direction */
#include "engine/math/number.h"
using entity_direction_t = number_t;

/** Entity health */
using entity_health_t = uint16_t;

/** Entity energy */
using entity_energy_t = int32_t;

/** Player ID */
using player_id_t = uint16_t;

/** Player mask */
using player_mask_t = uint64_t;

/** Money type */
using money_t = int64_t;

/** Type for tile flags */
using tile_flags_t = uint16_t;

/** Type for cost of path */
using path_cost_t = uint32_t;

/** Type for time intervals in ms, can be positive or negative */
using duration_t = int64_t;

/**
 * Size struct with shorts
 */
struct size_16_t {
    uint16_t width = 0;
    uint16_t height = 0;
};

/**
 * Type for key code
 */
using input_key_code_t = int32_t;

/**
 * Type for mouse button index
 */
using mouse_button_t = uint8_t;

/**
 * Struct for key input data
 */
struct input_key_t {
    input_key_code_t code = 0;
    bool press = false;
    bool repeat = false;
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
    bool meta = false;
};

#endif //OPENE2140_TYPES_H
