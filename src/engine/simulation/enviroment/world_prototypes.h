//
// Created by Ion Agorria on 26/05/19
//
#ifndef OPENE2140_WORLD_PROTOTYPES_H
#define OPENE2140_WORLD_PROTOTYPES_H

#include "core/common.h"

/**
 * Stores each initial player data in the world
 */
struct PlayerPrototype {
    /**
     * Player ID
     */
    player_id_t id;

    /**
     * Player initial money amount
     */
    money_t money;

    /**
     * Player alliances mask
     */
    player_id_t alliances;
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
    entity_direction_t direction;

    /**
     * Player which entity belongs
     */
    player_id_t player;
};

#endif //OPENE2140_WORLD_PROTOTYPES_H
