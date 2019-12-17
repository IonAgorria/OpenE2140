//
// Created by Ion Agorria on 13/06/19
//
#include "path_handler.h"
#include "engine/simulation/entity.h"
#include "engine/simulation/player.h"

PathHandler::PathHandler(Player* player): player(player) {
}

Player* PathHandler::getPlayer() {
    return player;
}

void PathHandler::removeRequests(entity_id_t entity_id) {
    for (const auto& request : requests) {
        if (request->removeEntity(entity_id)) {
            //No need to iterate the rest since an entity should belong to one request only
            break;
        }
    }
}

std::shared_ptr<PathRequest>
PathHandler::requestDestination(entity_id_t entity_id, Tile* tile, bool partial) {
    std::shared_ptr<PathRequest> activeRequest;
    removeRequests(entity_id);

    if (tile) {
        PathRequestMode mode = partial ? PathRequestMode::ACTIVE_PARTIAL
                                       : PathRequestMode::ACTIVE_TILE;

        //Attempt to find a existing request that can be reused if is not partial
        if (!partial) {
            for (const auto& request : requests) {
                if (request->mode == PathRequestMode::ACTIVE_TILE && request->getDestination() == tile) {
                    activeRequest = request;
                    break;
                }
            }
        }

        //None found, create new request
        if (!activeRequest) {
            activeRequest = std::make_shared<PathRequest>();
            activeRequest->mode = mode;
            activeRequest->handler = this;
            activeRequest->simulation = player->simulation;
            activeRequest->setDestination(tile);
            requests.push_back(activeRequest);
        }

        //Add entity to request
        activeRequest->addEntity(entity_id);
    }

    return activeRequest;
}

std::shared_ptr<PathRequest>
PathHandler::requestTarget(entity_id_t entity_id, const std::shared_ptr<Entity>& target) {
    std::shared_ptr<PathRequest> activeRequest;
    removeRequests(entity_id);

    if (target && target->isActive() && entity_id != target->getID()) {
        //Attempt to find a existing request that can be reused
        for (const auto& request : requests) {
            if (request->mode == PathRequestMode::ACTIVE_ENTITY && request->getTarget() == target) {
                activeRequest = request;
                break;
            }
        }

        //None found, create new request
        if (!activeRequest) {
            activeRequest = std::make_shared<PathRequest>();
            activeRequest->mode = PathRequestMode::ACTIVE_ENTITY;
            activeRequest->handler = this;
            activeRequest->simulation = player->simulation;
            activeRequest->setTarget(target);
            requests.push_back(activeRequest);
        }

        //Add entity to request
        activeRequest->addEntity(entity_id);
    }

    return activeRequest;
}

void PathHandler::update() {
    for (auto it = requests.begin(); it != requests.end(); ) {
        PathRequest* request = (*it).get();

        //Update
        request->update();

        //Remove request if no longer active
        if (request->mode == PathRequestMode::INACTIVE) {
            request->simulation = nullptr;
            request->handler = nullptr;
            it = requests.erase(it);
            continue;
        }

        //Move to next
        ++it;
    }
}
