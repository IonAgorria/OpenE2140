//
// Created by Ion Agorria on 22/8/19
//
#include "engine/core/utils.h"
#include "entity.h"
#include "entity_store.h"

void EntityStore::clear() {
    entities.clear();
}

std::shared_ptr<Entity> EntityStore::getEntity(entity_id_t id) const {
    std::shared_ptr<Entity> entity;
    auto it = entitiesMap.find(id);
    if (it != entitiesMap.end()) {
        entity = it->second; }
    return entity;
}

const std::vector<std::shared_ptr<Entity>>& EntityStore::getEntitiesByType(const entity_type_t& type) {
    return entitiesTypeMap[entityKeyFromType(type)];
}

const std::vector<std::shared_ptr<Entity>>& EntityStore::getEntities() const {
    return entities;
}

entity_id_t EntityStore::add(const std::shared_ptr<Entity>& entity) {
    lastEntityID++;
    entities.emplace_back(entity);
    entitiesMap[lastEntityID] = entity;
    return lastEntityID;
}

void EntityStore::remove(const std::shared_ptr<Entity>& entity) {
    Utils::eraseElementFromVector(entities, entity);
    auto it = entitiesMap.find(entity->getID());
    if (it != entitiesMap.end()) {
        entitiesMap.erase(it);
    }
}

entity_type_t EntityStore::entityTypeFromKey(entity_type_key_t key) {
    return {
            static_cast<entity_kind_t>((key >> 32) & 0xFFFFFFFF),
            static_cast<entity_type_id_t>(key & 0xFFFFFFFF),
    };
}

entity_type_key_t EntityStore::entityKeyFromType(const entity_type_t& type) {
    return (static_cast<entity_type_key_t>(type.kind) << 32)
           | static_cast<entity_type_key_t>(type.id);
}