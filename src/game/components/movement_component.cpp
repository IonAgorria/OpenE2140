//
// Created by Ion Agorria on 08/09/19
//

#include "engine/simulation/components/image_component.h"
#include "game/core/game.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/world/world.h"
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
        updateSpriteIndex(base);
        //Set the initial tile if none is set already
        if (base->getTiles().empty()) {
            Simulation* simulation =  base->getSimulation();
            World* world = simulation->getWorld();
            Tile* tile = world->getTile(base->getPosition());
            base->clearTiles();
            std::shared_ptr<Entity> entity = base->getEntityPtr();
            tile->addEntity(entity);
        }
    }
}

void MovementComponent::entityChanged() {
    updateSpriteIndex(base);
}

void MovementComponent::chooseSprite() {
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    if (state == MovementState::Moving && movementType == MovementType::GroundWalker) {
        imageComponent->setAnimationFromSprite("moving_" + std::to_string(spriteIndex));
    } else {
        imageComponent->setImageFromSprite("default_" + std::to_string(spriteIndex));
    }
}

MovementState MovementComponent::getState() {
    return state;
}
