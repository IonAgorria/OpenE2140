//
// Created by Ion Agorria on 15/01/20
//
#ifndef OPENE2140_ROTATION_COMPONENT_H
#define OPENE2140_ROTATION_COMPONENT_H

#include "engine/math/number.h"
#include "component.h"

class Entity;

/**
 * Stores self-rotation information
 */
class RotationComponent {
CLASS_COMPONENT(Entity, RotationComponent)
protected:
    /**
     * Target direction to turn
     */
    number_t targetDirection;

    /**
     * The amount of speed applied during rotation to reach target direction
     */
    number_t rotationSpeed;

public:
    /**
     * Get entity direction according to delta and target direction
     */
    number_t getDeltaDirection();

    /**
     * Set entity target direction
     *
     * @param newDirection to set
     */
    void setTargetDirection(number_t newDirection);

    /** @return entity target direction */
    number_t getTargetDirection();

    /** @return if current direction is the target direction */
    bool isTargetDirection();

    /**
     * Set entity rotation speed
     *
     * @param newRotationSpeed to set
     */
    void setRotationSpeed(number_t newRotationSpeed);

    /** @return entity rotation speed */
    number_t getRotationSpeed();
};

#endif //OPENE2140_ROTATION_COMPONENT_H
