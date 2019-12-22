//
// Created by Ion Agorria on 14/06/19
//
#include "engine/simulation/simulation.h"
#include "src/engine/simulation/entity.h"
#include "src/engine/simulation/entity_store.h"
#include "engine/simulation/world/world.h"
#include "engine/simulation/world/tile.h"
#include "path_request.h"

PathRequest::PathRequest() {
}

World* PathRequest::getWorld() const {
    return handler && simulation ? simulation->getWorld() : nullptr;
}

void PathRequest::initialize() {
    //Reset each vertex
    World* world = this->getWorld();
    if (world) {
        auto& tiles = world->getTiles();
        vertexes.resize(tiles.size());
        for (std::unique_ptr<Tile>& tile : tiles) {
            PathVertex& vertex = vertexes[tile->index];
            vertex.index = tile->index;
            vertex.back = 0;
            vertex.g = PATHFINDER_INFINITY;
            vertex.l = 0;
        }
    } else {
        vertexes.clear();
    }
    //Init each pathfinders
    for (auto& pair : pathfinders) {
        pair.second->initialize();
    }
}

const std::vector<PathVertex>& PathRequest::getVertexes() const {
    return vertexes;
}

std::vector<PathVertex>& PathRequest::getVertexes() {
    return vertexes;
}

bool PathRequest::addEntity(entity_id_t entity) {
    std::unique_ptr<AStar>& pathfinder = pathfinders[entity];
    if (pathfinder) {
        //Already exists in request
        return false;
    }
    pathfinder = std::make_unique<AStar>(this);
    pathfinders[entity] = std::move(pathfinder);
    return true;
}

bool PathRequest::removeEntity(entity_id_t entity) {
    return pathfinders.erase(entity) != 0;
}

PathFinderStatus PathRequest::getResult(entity_id_t entity, std::vector<tile_index_t> path) const {
    if (mode == PathRequestMode::INACTIVE) {
        return PathFinderStatus::None;
    }
    const auto pathfinder = pathfinders.find(entity);
    if (pathfinder == pathfinders.end()) return PathFinderStatus::None;
    PathFinderStatus status = pathfinder->second->getStatus();
    if (status == PathFinderStatus::Partial
     || status == PathFinderStatus::Success) {
        //TODO do the path filling if pathfinderstatus is partial or complete
    }
    return status;
}

Tile* PathRequest::getDestination() const {
    return destination;
}

void PathRequest::setDestination(Tile* newDestination) {
    if (mode == PathRequestMode::INACTIVE) {
        return;
    }

    if (destination != newDestination) {
        destination = newDestination;
        initialize();
    }
}

std::shared_ptr<Entity> PathRequest::getTarget() const {
    return target;
}

void PathRequest::setTarget(std::shared_ptr<Entity> entity) {
    if (mode == PathRequestMode::ACTIVE_ENTITY) {
        //No need to initialize, since destination will be updated automatically
        target = std::move(entity);
    }
}

bool PathRequest::empty() {
    return pathfinders.empty();
}

void PathRequest::update() {
    //Skip if mode is inactive
    if (mode == PathRequestMode::INACTIVE) {
        return;
    }

    //If it has a target attempt to get the tile to handle any possible changes
    if (target) {
        bool valid = false;
        if (target->isActive()) {
            Tile* tile = target->getTile();
            if (tile) {
                setDestination(tile);
                valid = true;
            }
        }

        //No longer check
        if (!valid) {
            target.reset();
            //If there is a destination set already then switch to active tile
            if (destination) {
                mode = PathRequestMode::ACTIVE_TILE;
            }
        }
    }

    //If there is no destination remove all pathfinders
    if (!destination) {
        pathfinders.clear();
    }

    //Check if there is anything left
    if (empty()) {
        mode = PathRequestMode::INACTIVE;
        return;
    }

    //Update each pathfinders
    auto entityStore = simulation->getEntitiesStore();
    for (auto it = pathfinders.begin(); it != pathfinders.end(); ) {
        //Remove if entity is no longer active
        std::shared_ptr<Entity> entity = entityStore->getEntity(it->first);
        Tile* tile = entity->getTile();
        if (!entity || !entity->isActive() || !tile) {
            it = pathfinders.erase(it);
            continue;
        }

        //Ignore if not computing
        auto& pathfinder = it->second;
        auto status = pathfinder->getStatus();
        if (status == PathFinderStatus::None || status == PathFinderStatus::Computing) {
            //Update state according to partial mode
            if (this->mode == PathRequestMode::ACTIVE_PARTIAL) {
                pathfinder->plan(tile, destination);
            } else {
                pathfinder->plan(destination, tile);
            }

            //Update computation
            pathfinder->compute();
        }

        //Move to next
        ++it;
    }
}