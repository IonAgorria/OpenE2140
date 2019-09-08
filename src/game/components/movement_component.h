//
// Created by Ion Agorria on 08/09/19
//
#ifndef OPENE2140_MOVEMENT_COMPONENT_H
#define OPENE2140_MOVEMENT_COMPONENT_H

#include "engine/core/types.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/entity.h"

/**
 * Movement state
 */
enum class MovementState {
    Standby,
    WaitPathfinder,
    Floating,
    ChangeAltitude,
    Rotating,
    Moving,
};

/**
 * Movement type
 */
enum class MovementType {
    GroundWheel,
    GroundTank,
    GroundWalker,
    Air,
    Water
};

/**
 * Controls the movement and rotation of entity
 * Updates the sprites according to entity type and movement state
 */
CLASS_COMPONENT(Entity, MovementComponent)
protected:

    /**
     * The sprite rotation index
     */
    uint8_t rotationIndex = 0;

    /**
     * State which entity is currently
     */
    MovementState state;

    /**
     * Selects current sprite
     */
    void chooseSprite();

public:

    /** @neturn entity movement state */
    MovementState getState();
};

#endif //OPENE2140_MOVEMENT_COMPONENT_H
