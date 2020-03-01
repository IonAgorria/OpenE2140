//
// Created by Ion Agorria on 08/09/19
//

#include <src/engine/simulation/components/rotation_component.h>
#include "engine/simulation/components/player_component.h"
#include "engine/simulation/components/image_component.h"
#include "game/core/game.h"
#include "engine/entities/entity_config.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/world/world.h"
#include "movement_component.h"

CLASS_COMPONENT_DEFAULT(MovementComponent)

PathHandler* getPathHandler(Entity* entity) {
    //Get player component, if missing then don't add this entity
    PlayerComponent* component = GET_COMPONENT_DYNAMIC(entity, PlayerComponent);
    Player* player = component ? component->getPlayer() : nullptr;
    if (!player) {
        return nullptr;
    }
    return player->pathHandler.get();
}

void MovementComponent::dispatchPathTile() {
    const Tile* currentTile = path.empty() ? nullptr : path.back();
    if (currentTile) {
        //Get distance
        Vector2 targetPosition;
        base->getSimulation()->toWorldVector(currentTile->position, targetPosition, true);
        number_t distance = base->getPosition().distance(targetPosition);

        //Already on position, skip it
        if (NUMBER_ZERO >= distance) {
            path.pop_back();
            dispatchPathTile();
            return;
        }
        RotationComponent* rotationComponent = GET_COMPONENT_DYNAMIC(base, RotationComponent);
        if (rotationComponent) {
            //Check if target angle is correct, else rotate it
            number_t angle = base->getPosition().getAngle(targetPosition);
            rotationComponent->setTargetDirection(angle);
            if (rotationComponent->isTargetDirection()) {
                setStateTo(MovementState::Moving);
            } else {
                setStateTo(MovementState::Rotating);
            }
        } else {
            setStateTo(MovementState::Moving);
        }
    } else {
        //Reached end?
        setStateTo(MovementState::Standby);
    }
}

void MovementComponent::setStateTo(MovementState newState) {
    //Only run if state changes
    if (state == newState) return;
    LOG_DEBUG("setStateTo {0} -> {1}", (unsigned int) (state), (unsigned int) (newState));

    //Handle states
    switch (newState) {
        case MovementState::Standby:
            stop();
            break;
        case MovementState::WaitPathfinder:
            path.clear();
            break;
        default:
            break;
    }

    state = newState;
}

void MovementComponent::update() {
    //Check if path request is inactive
    if (pathRequest && pathRequest->mode == PathRequestMode::INACTIVE) {
        setStateTo(MovementState::Standby);
    }

    switch (state) {
        case MovementState::Standby:
            break;
        case MovementState::WaitPathfinder:
            //Check status if there is a request and path is empty
            if (pathRequest) {
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
                            pathRequest = pathRequest->requestPartial(base->getEntityPtr());
                        } else {
                            pathRequest = nullptr;
                        }
                        break;
                    case PathFinderStatus::Success:
                    case PathFinderStatus::Partial:
                        dispatchPathTile();
                        break;
                }
            }

            //Unset WaitPathfinder if no request is pending
            if (!pathRequest) {
                setStateTo(MovementState::Standby);
            }
            break;
        case MovementState::ChangeAltitude:
            //TODO implement this for air units
            break;
        case MovementState::Rotating: {
            RotationComponent* rotationComponent = GET_COMPONENT_DYNAMIC(base, RotationComponent);
            if (!rotationComponent || rotationComponent->isTargetDirection()) {
                dispatchPathTile();
            }
            break;
        }
        case MovementState::Moving:
            const Tile* currentTile = path.back();
            if (currentTile) {
                //Get factor of lerp
                number_t factor = NUMBER_ZERO;
                Vector2 targetPosition;
                base->getSimulation()->toWorldVector(currentTile->position, targetPosition, true);
                if (NUMBER_ZERO < getForwardSpeed()) {
                    number_t distance = base->getPosition().distance(targetPosition);
                    number_t speed = number_mul(getForwardSpeed(), GAME_DELTA);
                    if (speed < distance) {
                        factor = number_div(speed, distance);
                    } else {
                        factor = NUMBER_ONE;
                    }
                }

                //There is movement to apply?
                if (NUMBER_ZERO < factor) {
                    //Check if factor goes beyond goal position
                    bool reach = NUMBER_ONE <= factor;
                    if (reach) factor = NUMBER_ONE;

                    //Apply position
                    Vector2 result;
                    base->getPosition().lerp(targetPosition, factor, result);
                    base->setPosition(result);

                    //Next
                    if (reach) {
                        path.pop_back();
                        dispatchPathTile();
                    }
                }
            } else {
                dispatchPathTile();
            }
            break;
    }
}

void MovementComponent::setup() {
    const EntityConfig* config = base->getConfig();
    forwardSpeed = float_to_number(config->getData<float>("forward_speed", 0.0));
    altitudeSpeed = float_to_number(config->getData<float>("altitude_speed", 0.0));

    //Set movement type
    const std::string& entType = config->type;
    if (entType == "walker") {
        movementType = MovementType::GroundWalker;
        base->tileFlagsRequired = TILE_FLAG_PASSABLE;
        base->entityFlagsMask = TILE_FLAG_ENTITY_TERRAIN;
    } else if (entType == "tank" || entType == "mcu") {
        movementType = MovementType::GroundTank;
        base->tileFlagsRequired = TILE_FLAG_PASSABLE;
        base->entityFlagsMask = TILE_FLAG_ENTITY_TERRAIN;
    } else if (entType == "amphibian") {
        movementType = MovementType::Amphibian;
        base->tileFlagsRequired = TILE_FLAG_PASSABLE;
        base->entityFlagsMask = TILE_FLAG_ENTITY_TERRAIN;
    } else if (entType == "vtol" || entType == "bomber" || entType == "helicopter") {
        movementType = MovementType::Air;
        base->tileFlagsRequired = 0;
        base->entityFlagsMask = TILE_FLAG_ENTITY_AIR;
    } else if (entType == "ship" || entType == "submarine") {
        movementType = MovementType::Water;
        base->tileFlagsRequired = TILE_FLAG_PASSABLE | TILE_FLAG_WATER;
        base->entityFlagsMask = TILE_FLAG_ENTITY_TERRAIN;
    } else {
        movementType = MovementType::Ground;
        base->tileFlagsRequired = TILE_FLAG_PASSABLE;
        base->entityFlagsMask = TILE_FLAG_ENTITY_TERRAIN;
    }
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
    } else {
        //Reset state
        setStateTo(MovementState::Standby);
    }
}

void MovementComponent::entityChanged() {
    updateSpriteIndex(base);
}

void MovementComponent::chooseSprite() {
    ImageComponent* imageComponent = GET_COMPONENT_DYNAMIC(base, ImageComponent);
    if (state == MovementState::Moving && movementType == MovementType::GroundWalker) {
        imageComponent->setAnimationFromSprite("moving_" + std::to_string(spriteIndex));
    } else {
        imageComponent->setImageFromSprite("default_" + std::to_string(spriteIndex));
    }
}

MovementState MovementComponent::getState() {
    return state;
}

bool MovementComponent::isIdle() {
    return state == MovementState::Standby;
}

void MovementComponent::stop() {
    //Remove any pending path
    path.clear();

    //Remove ourselves from request if any
    if (pathRequest) {
        pathRequest->removeEntity(base->getID());
        pathRequest = nullptr;
    }

    //Stop any ongoing rotation
    if (state == MovementState::Rotating) {
        RotationComponent* rotationComponent = GET_COMPONENT_DYNAMIC(base, RotationComponent);
        if (rotationComponent) {
            rotationComponent->setTargetDirection(base->getDirection());
        }
    }
}

void MovementComponent::move(Tile* tile) {
    entity_ptr entityPtr = base->getEntityPtr();
    PathHandler* pathHandler = getPathHandler(base);
    pathRequest = pathHandler->requestDestination(entityPtr, tile);
    setStateTo(MovementState::WaitPathfinder);
}

void MovementComponent::follow(const std::shared_ptr<Entity>& entity) {
    entity_ptr entityPtr = base->getEntityPtr();
    PathHandler* pathHandler = getPathHandler(base);
    pathRequest = pathHandler->requestTarget(entityPtr, entity);
    setStateTo(MovementState::WaitPathfinder);
}

number_t MovementComponent::getForwardSpeed() {
    return forwardSpeed;
}

number_t MovementComponent::getAltitudeSpeed() {
    return altitudeSpeed;
}
