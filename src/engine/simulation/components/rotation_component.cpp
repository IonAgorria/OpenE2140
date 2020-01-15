//
// Created by Ion Agorria on 15/01/20
//

#include "engine/simulation/entity.h"
#include "rotation_component.h"

CLASS_COMPONENT_DEFAULT(RotationComponent)

void RotationComponent::setup() {
}

void RotationComponent::simulationChanged() {
}

void RotationComponent::update() {
    if (rotationSpeed != 0 && !isTargetDirection()) {
        base->setDirection(getDeltaDirection());
    }
}

void RotationComponent::entityChanged() {
}

/**
 * Get entity direction according to delta and target direction
 *
 * @return direction
 */
number_t RotationComponent::getDeltaDirection() {
    //Get the required angle to rotate and limit by rotation speed
    number_t direction = base->getDirection();
    number_t angle = number_angle_diff(getTargetDirection(), direction);
    number_t limit = number_mul(getRotationSpeed(), GAME_DELTA);
    number_t step = std::clamp(angle, -limit, limit);
    direction = number_add(direction, step);
    return number_wrap_angle(direction);
}

void RotationComponent::setTargetDirection(number_t newDirection) {
    targetDirection = number_wrap_angle(newDirection);
    base->changesCount++;
}

float RotationComponent::getTargetDirection() {
    return targetDirection;
}

bool RotationComponent::isTargetDirection() {
    return base->getDirection() == targetDirection;
}

void RotationComponent::setRotationSpeed(number_t newRotationSpeed) {
    rotationSpeed = newRotationSpeed;
    base->changesCount++;
}

float RotationComponent::getRotationSpeed() {
    return rotationSpeed;
}
