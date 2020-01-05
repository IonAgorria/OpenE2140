//
// Created by Ion Agorria on 08/09/19
//
#ifndef OPENE2140_MOVEMENT_COMPONENT_H
#define OPENE2140_MOVEMENT_COMPONENT_H

#include "sprite_rotation_component.h"

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
    Ground,
    GroundTank,
    GroundWalker,
    Amphibian,
    Air,
    Water
};

/**
 * Controls the movement and rotation of entity
 * Updates the sprites according to entity type and movement state
 */
class MovementComponent: public SpriteRotationComponentCommon {
CLASS_COMPONENT(Entity, MovementComponent)
protected:
    /**
     * State which entity is currently
     */
    MovementState state = MovementState::Standby;

    /**
     * Type of movement
     */
    MovementType movementType = MovementType::Ground;

    /**
     * Current pathfinder request
     */
    std::shared_ptr<PathRequest> pathRequest = nullptr;

    /**
     * Current planned path to follow
     */
    std::vector<const Tile*> path;

    /*
     * SpriteRotationComponentCommon
     */

    void chooseSprite() override;

public:
    /** @neturn entity movement state */
    MovementState getState();
};

#endif //OPENE2140_MOVEMENT_COMPONENT_H
