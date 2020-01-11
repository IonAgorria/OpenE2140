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

    /**
     * Called when new movement state is set
     *
     * @param newState
     */
    void setStateTo(MovementState newState);

    /**
     * Called when next node must be handled in path
     */
    void dispatchPathTile();

    /*
     * SpriteRotationComponentCommon
     */

    void chooseSprite() override;

public:
    /** @neturn entity movement state */
    MovementState getState();

    /** @return if entity is idle without any pending stuff */
    bool isIdle();

    /**
     * Tells the movement component to stop any movement
     *
     * This doesn't stop any ongoing rotation/movement
     */
    void stop();

    /**
     * Tells the movement component to start moving the entity to target tile
     *
     * @param tile
     */
    void move(Tile* tile);

    /**
     * Tells the movement component to start following the entity to target entity
     *
     * @param entity
     */
    void follow(const std::shared_ptr<Entity>& entity);
};

#endif //OPENE2140_MOVEMENT_COMPONENT_H
