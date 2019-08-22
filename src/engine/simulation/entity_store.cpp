//
// Created by Ion Agorria on 22/8/19
//
#include "engine/core/utils.h"
#include "entity.h"
#include "entity_store.h"

#include <utility>

#define ENTITY_STORE_LOOKNEAR 10

void EntityStore::clear() {
    entities.clear();
}

std::shared_ptr<Entity> EntityStore::getEntity(entity_id_t id) const {
    std::shared_ptr<Entity> entity;
    if (0 < id && id < entities.size()) {
        entity = entities[id];
    }
    return entity;
}

const std::vector<std::shared_ptr<Entity>>& EntityStore::getEntities() const {
    return entities;
}

entity_id_t EntityStore::add(std::shared_ptr<Entity> entity) {
    updateLowestID();
    lastEntityID = lowestID;
    entities[lowestID] = std::move(entity);
    return lastEntityID;
}

void EntityStore::remove(std::shared_ptr<Entity> entity) {
    Utils::eraseElementFromVector(entities, entity);
}

void EntityStore::updateLowestID() {
    //Check if current ID is already valid by chance
    if (0 < lowestID
    && lowestID < entities.size()
    && !entities[lowestID]) {
        return;
    }

    //First next try by searching some near entries if they are empty
    entity_id_t minID = lastEntityID < ENTITY_STORE_LOOKNEAR ? 1 : lastEntityID - ENTITY_STORE_LOOKNEAR;
    entity_id_t maxID = std::min(lastEntityID + ENTITY_STORE_LOOKNEAR, static_cast<entity_id_t>(entities.size()));
    for (lowestID = minID; lowestID < maxID; ++lowestID) {
        if (!entities[lowestID]) {
            //Free, stop
            return;
        }
    }

    //Lets try transversing the entire array
    for (lowestID = 1; lowestID < entities.size(); ++lowestID) {
        if (!entities[lowestID]) {
            //Free, stop
            return;
        }
    }

    //None found, lets grow it and set the lowest ID too
    lowestID = entities.empty() ? 1 : entities.size();
    entities.resize(lowestID + 1);
}
