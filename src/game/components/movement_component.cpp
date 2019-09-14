//
// Created by Ion Agorria on 08/09/19
//

#include "engine/simulation/components/image_component.h"
#include "game/core/constants.h"
#include "game/core/game.h"
#include "movement_component.h"

CLASS_COMPONENT_DEFAULT(MovementComponent)

void MovementComponent::update() {
    base->setDirection(number_add(base->getDirection(), float_to_number(0.005)));
}

void MovementComponent::setup() {
}

void MovementComponent::simulationChanged() {
    if (base->isActive()) {
        //Setup sprite
        updateSpriteRotation(base);
    }
}

void MovementComponent::entityChanged() {
    updateSpriteRotation(base);
}

void MovementComponent::chooseSprite() {
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    if (state == MovementState::Moving && movementType == MovementType::GroundWalker) {
        imageComponent->setAnimationFromSprite("moving_" + std::to_string(rotationIndex));
    } else {
        imageComponent->setImageFromSprite("default_" + std::to_string(rotationIndex));
    }
}

MovementState MovementComponent::getState() {
    return state;
}
