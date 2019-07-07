//
// Created by Ion Agorria on 23/06/19
//

#include "entity_factory.h"
#include "entity_manager.h"

EntityManager::EntityManager(std::shared_ptr<Engine> engine): engine(engine) {
    log = Log::get("Entities");
}

EntityManager::~EntityManager() {
    log->debug("Closing");
    factories.clear();
}

void EntityManager::addEntityFactory(std::unique_ptr<IEntityFactory> factory) {
    factory->setManager(this);
    entity_kind_t kind = factory->getKind();
    factories[kind] = std::move(factory);
}

void EntityManager::clear() {
    //Send clear to each registered factories
    for (std::unordered_map<entity_kind_t, std::unique_ptr<IEntityFactory>>::iterator pair = factories.begin(); pair != factories.end(); ++pair) {
        pair->second->clear();
    }
}

void EntityManager::load() {
    //Clear
    clear();

    //Load each registered factories
    for (std::unordered_map<entity_kind_t, std::unique_ptr<IEntityFactory>>::iterator pair = factories.begin(); pair != factories.end(); ++pair) {
        std::unique_ptr<IEntityFactory>& factory = pair->second;
        factory->load();
        error = factory->getError();
        if (hasError()) {
            log->error("Error loading factory with kind {0}:\n{1}", pair->first, error);
            return;
        }
    }
}

std::shared_ptr<Entity> EntityManager::makeEntity(entity_type_t type) {
    std::unique_ptr<IEntityFactory>& factory = factories[type.kind];
    std::shared_ptr<Entity> entity;
    if (factory) {
        entity = factory->makeEntity(type.id);
        if (factory->hasError()) {
            log->warn("Error when making new entity with type {0}:{1}:\n{2}", type.kind, type.id, factory->getError());
        }
    }
    return entity;
}