//
// Created by Ion Agorria on 23/06/19
//

#include "engine/core/engine.h"
#include "entity_factory.h"
#include "entity_manager.h"

EntityManager::EntityManager(std::shared_ptr<Engine> engine): engine(engine) {
    log = Log::get("Entities");
}

EntityManager::~EntityManager() {
    log->debug("Closing");
    clear();
    factories.clear();
}

void EntityManager::addEntityFactory(std::unique_ptr<IEntityFactory> factory) {
    factory->setManager(this);
    entity_kind_t kind = factory->getKind();
    //Allocate new empty if not enough are present
    if (factories.size() < kind + 1) {
        factories.resize(kind + 1);
    }
    factories[kind].swap(factory);
}

void EntityManager::clear() {
    //Send clear to each registered factories
    for (std::unique_ptr<IEntityFactory>& factory : factories) {
        if (!factory) continue;
        factory->clear();
    }
}

void EntityManager::load() {
    //Clear
    clear();

    //Load each registered factories
    for (std::unique_ptr<IEntityFactory>& factory : factories) {
        if (!factory) continue;
        factory->load();
        error = factory->getError();
        if (hasError()) {
            log->error("Error loading factory with kind {0}:\n{1}", factory->getKind(), error);
            return;
        }
    }

    log->debug("Loaded entity manager");
}

std::shared_ptr<Entity> EntityManager::makeEntity(const entity_type_t& type) {
    std::unique_ptr<IEntityFactory>& factory = factories[type.kind];
    std::shared_ptr<Entity> entity;
    if (factory) {
        entity = factory->makeEntity(type);
        if (factory->hasError()) {
            log->warn("Error when making new entity with type {0} {1} {2}:\n{3}", type.kind, type.id, type.code, factory->getError());
        }
    }
    return entity;
}

AssetManager* EntityManager::getAssetManager() {
    return engine->getAssetManager();
}