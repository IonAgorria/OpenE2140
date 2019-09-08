//
// Created by Ion Agorria on 08/09/19
//

#include "engine/simulation/components/image_component.h"
#include "movement_component.h"

CLASS_COMPONENT_DEFAULT(MovementComponent)

void MovementComponent::update() {
    //Handle rotation
}

void MovementComponent::setup() {
}

void MovementComponent::simulationChanged() {
    if (base->isActive()) {
        chooseSprite();
    }
}

void MovementComponent::entityChanged() {
    //Check if anything changed to update sprite
    uint8_t rotationIndexNew = 0; //TODO calculate this from rotation
    if (rotationIndex != rotationIndexNew) {
        rotationIndex = rotationIndexNew;
        chooseSprite();
    }
}

void MovementComponent::chooseSprite() {
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    if (state == MovementState::Moving) {
        imageComponent->setAnimationFromSprite("moving_" + std::to_string(rotationIndex));
    } else {
        imageComponent->setImageFromSprite("default_" + std::to_string(rotationIndex));
    }
}