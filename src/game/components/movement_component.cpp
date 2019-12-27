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
    //Check if path request is inactive
    if (pathRequest && pathRequest->mode == PathRequestMode::INACTIVE) {
        pathRequest = nullptr;
    }

    //Check status if there is a request and path is empty
    if (pathRequest && path.empty()) {
        entity_id_t entityId = base->getID();
        PathFinderStatus status = pathRequest->getResult(entityId, path);
        switch (status) {
            default:
            case PathFinderStatus::Computing:
                break;
            case PathFinderStatus::None:
                pathRequest = nullptr;
                break;
            case PathFinderStatus::Fail:
                //Request partial request if request is not already partial
                if (pathRequest->mode != PathRequestMode::ACTIVE_PARTIAL) {
                    pathRequest = pathRequest->requestPartial(entityId);
                } else {
                    pathRequest = nullptr;
                }
                break;
            case PathFinderStatus::Success:
            case PathFinderStatus::Partial:
                break;
        }
    }

    //Handle non empty path
    if (!path.empty()) {

    }

    //TODO remove this
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
