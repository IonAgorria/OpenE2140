//
// Created by Ion Agorria on 26/05/19
//
#ifndef OPENE2140_WORLD_PROTOTYPES_H
#define OPENE2140_WORLD_PROTOTYPES_H

#include "engine/math/vector2.h"
#include "engine/core/common.h"

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
    tile_flags_t tileFlags = 0;

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
     * Player ID
     */
    player_id_t id = 0;

    /**
     * Player enemies mask, those which are enemies are marked as 1 in their bit position (determined at mask)
     */
    player_mask_t enemies = 0;

    /**
     * Player initial money amount
     */
    money_t money = 0;

    /**
     * Player faction
     */
    faction_id_t faction = 0;
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
