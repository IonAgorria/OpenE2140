//
// Created by Ion Agorria on 26/05/19
//
#ifndef OPENE2140_ENTITY_PROTOTYPE_H
#define OPENE2140_ENTITY_PROTOTYPE_H

#include "core/common.h"

/**
 * Stores each entity that is going to be spawned in the world
 */
class EntityPrototype {
public:
    /**
     * Type of entity
     */
    entity_type type;

    /**
     * Player which entity belongs
     */

    /**
     * Constructor
     */
    EntityPrototype();

    /**
     * Destructor
     */
    ~EntityPrototype() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EntityPrototype)
};

#endif //OPENE2140_TILE_H
