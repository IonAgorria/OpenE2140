//
// Created by Ion Agorria on 26/05/19
//
#ifndef OPENE2140_WORLD_PROTOTYPES_H
#define OPENE2140_WORLD_PROTOTYPES_H

#include "core/common.h"

/**
 * Stores each initial tile data in the world
 */
struct TilePrototype {
    /**
     * Index of tile to use in tileset
     */
    unsigned int index = 0;

    /**
     * Flag for water
     */
    bool isWater = false;

    /**
     * Flag for shore
     */
    bool isShore = false;

    /**
     * Flag for passable/ground
     */
    bool isPassable = false;

    /**
     * Flag for immutable (is not affected by explosions or other stuff
     */
    bool isImmutable = false;

    /**
     * Contained ore in this tile
     */
    money_t ore = 0;

    /**
     * Flag for sand
     */
    bool isSand = false;
};

/**
 * Stores each initial player data in the world
 */
struct PlayerPrototype {
    /**
     * Player ID
     */
    player_id_t id = 0;

    /**
     * Player initial money amount
     */
    money_t money = 0;

    /**
     * Player alliances mask
     */
    player_id_t alliances = 0;
};

/**
 * Stores each entity that is going to be spawned in the world
 */
struct EntityPrototype {
    /**
     * Type of entity
     */
    entity_type_t type;

    /**
     * Direction of entity
     */
    entity_direction_t direction = 0;

    /**
     * Player which entity belongs
     */
    player_id_t player = 0;
};

#endif //OPENE2140_WORLD_PROTOTYPES_H
