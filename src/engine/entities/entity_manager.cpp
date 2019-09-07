//
// Created by Ion Agorria on 23/06/19
//

#include "engine/core/engine.h"
#include "entity_factory.h"
#include "entity_manager.h"
#include "entity_config.h"
#include "engine/simulation/entity.h"

EntityManager::EntityManager(std::shared_ptr<Engine> engine): engine(std::move(engine)) {
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

std::shared_ptr<Entity> EntityManager::makeEntity(EntityConfig* config) {
    std::shared_ptr<Entity> entity;
    if (config) {
        std::unique_ptr<IEntityFactory>& factory = factories[config->kind];
        if (factory) {
            entity = factory->instanceEntity(config);
            error = factory->getError();
            if (entity) {
                entity->setup(config);
            }
            if (hasError()) {
                log->warn("Error when making new entity with config {0}:\n{1}", config->toString(), getError());
            }
        }
    } else {
        BUG("Attempted to make a new entity with null config");
    }
    return entity;
}

std::shared_ptr<Entity> EntityManager::makeEntity(const entity_type_t& type) {
    std::unique_ptr<IEntityFactory>& factory = factories[type.kind];
    std::shared_ptr<Entity> entity;
    if (factory) {
        EntityConfig* config = factory->getConfig(type.id);
        if (config) {
            entity = makeEntity(config);
        } else {
            log->warn("Attempted to make a new entity with missing config for {0}:{1}", type.kind, type.id);
        }
    } else {
        BUG("Attempted to make a new entity with missing factory for {0}:{1}", type.kind, type.id);
    }
    return entity;
}

std::shared_ptr<Entity> EntityManager::makeEntity(entity_kind_t kind, const std::string& code) {
    std::unique_ptr<IEntityFactory>& factory = factories[kind];
    std::shared_ptr<Entity> entity;
    if (factory) {
        EntityConfig* config = factory->getConfigCode(code);
        entity = makeEntity(config);
    }
    return entity;
}

AssetManager* EntityManager::getAssetManager() {
    return engine->getAssetManager();
}