//
// Created by Ion Agorria on 14/06/19
//
#include "engine/simulation/simulation.h"
#include "engine/simulation/world/world.h"
#include "engine/simulation/entities/entity.h"
#include "engine/simulation/world/tile.h"
#include "path_request.h"

PathRequest::PathRequest() {
}

void PathRequest::initialize() {
    //Reset each vertex
    auto& tiles = simulation->getWorld()->getTiles();
    vertexes.resize(tiles.size());
    for (std::unique_ptr<Tile>& tile : tiles) {
        PathVertex& vertex = vertexes[tile->index];
        vertex.index = tile->index;
        vertex.back = 0;
        vertex.g = PATHFINDER_INFINITY;
        vertex.l = 0;
    }
    //Init each pathfinders
    for (auto& pair : pathfinders) {
        pair.second->initialize();
    }
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
    pathfinder->initialize();
    pathfinders[entity] = std::move(pathfinder);
    return true;
}

bool PathRequest::removeEntity(entity_id_t entity) {
    return pathfinders.erase(entity) != 0;
}

PathFinderStatus PathRequest::getResult(entity_id_t entity, std::vector<tile_index_t> path) {
    std::unique_ptr<AStar>& pathfinder = pathfinders[entity];
    if (!pathfinder) return PathFinderStatus::None;
    PathFinderStatus status = pathfinder->getStatus();
    if (status == PathFinderStatus::Partial
     || status == PathFinderStatus::Success) {
        //TODO do the path filling if pathfinderstatus is partial or complete
    }
    return status;
}

void PathRequest::setDestination(Tile* newDestination) {
    if (destination != newDestination) {
        destination = newDestination;
        initialize();
    }
}

void PathRequest::setTarget(std::shared_ptr<Entity> entity) {
    target = entity;
}

bool PathRequest::empty() {
    return pathfinders.empty();
}

void PathRequest::update() {
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
        }
    }

    //If there is no destination remove all pathfinders
    if (!destination) {
        pathfinders.clear();
    }

    //Check if there is anything left
    if (empty()) return;

    //Update each pathfinders
    for (auto& pair : pathfinders) {
        pair.second->compute();
    }
}