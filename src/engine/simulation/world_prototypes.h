//
// Created by Ion Agorria on 26/05/19
//
#ifndef OPENE2140_WORLD_PROTOTYPES_H
#define OPENE2140_WORLD_PROTOTYPES_H

#include "math/vector2.h"
#include "core/common.h"

/**
 * Stores each initial tile data in the world
 */
struct TilePrototype {
    /**
     * Index of tile to use in tileset
     */
    unsigned int tilesetIndex = 0;

    /**
     * Tile flags
     */
    tile_flags_t tileFlags;

    /**
     * Contained ore in this tile
     */
    money_t ore = 0;
};

/**
 * Stores each initial player data in the world
 */
struct PlayerPrototype {
    /**
     * Player ID, each player has a bit
     */
    player_id_t id = 0;

    /**
     * Player enemies mask, those which are enemies are marked as 1 in their bit position (determined at id)
     */
    player_id_t enemies = 0;

    /**
     * Player initial money amount
     */
    money_t money = 0;

    /**
     * Player faction
     */
    unsigned int faction = 0;
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
     * Position of entity
     */
    Vector2 position;

    /**
     * Player which entity belongs
     */
    player_id_t player = 0;

    /**
     * Entity exists initially?
     */
    bool exists = true;

    /**
     * Entity is disabled?
     */
    bool disabled = false;
};

#endif //OPENE2140_WORLD_PROTOTYPES_H
