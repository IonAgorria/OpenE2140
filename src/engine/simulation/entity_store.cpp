//
// Created by Ion Agorria on 22/8/19
//
#include "engine/core/utils.h"
#include "entity.h"
#include "entity_store.h"
#include "engine/entities/entity_config.h"

void EntityStore::clear() {
    entities.clear();
    entitiesMap.clear();
    entitiesType.clear();
}

std::shared_ptr<Entity> EntityStore::getEntity(entity_id_t id) const {
    std::shared_ptr<Entity> entity;
    auto it = entitiesMap.find(id);
    if (it != entitiesMap.end()) {
        entity = it->second;
    }
    return entity;
}

std::vector<std::shared_ptr<Entity>>* EntityStore::getEntitiesByType(const entity_type_t& type) {
    //Allocate new if kind is higher
    if (type.kind + 1 >= entitiesType.size()) {
        entitiesType.resize(type.kind + 1);
    }
    //Allocate new if type id is higher
    auto& ents = entitiesType[type.kind];
    if (type.id + 1 >= ents.size()) {
        ents.resize(type.id + 1);
    }
    //Get the vector for this entity type
    return &ents[type.id];
}

const std::vector<std::vector<std::shared_ptr<Entity>>>* EntityStore::getEntitiesByKind(entity_kind_t kind) const {
    if (kind < entitiesType.size()) {
        return &entitiesType[kind];
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Entity>>* EntityStore::getEntitiesByType(const entity_type_t& type) const {
    if (type.kind < entitiesType.size()) {
        auto& ents = entitiesType[type.kind];
        if (type.id < ents.size()) {
            return &ents[type.id];
        }
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Entity>>& EntityStore::getEntities() const {
    return entities;
}

entity_id_t EntityStore::add(const std::shared_ptr<Entity>& entity) {
    lastEntityID++;
    entities.emplace_back(entity);
    entitiesMap[lastEntityID] = entity;
    std::vector<std::shared_ptr<Entity>>& byType = *getEntitiesByType(*entity->getConfig());
    byType.emplace_back(entity);
    return lastEntityID;
}

void EntityStore::remove(const std::shared_ptr<Entity>& entity) {
    std::vector<std::shared_ptr<Entity>>& byType = *getEntitiesByType(*entity->getConfig());
    Utils::eraseElementFromVector(byType, entity);
    auto it = entitiesMap.find(entity->getID());
    if (it != entitiesMap.end()) {
        entitiesMap.erase(it);
    }
    Utils::eraseElementFromVector(entities, entity);
}