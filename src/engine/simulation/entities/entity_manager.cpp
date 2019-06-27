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
        pair->second->load();
    }
}

std::shared_ptr<Entity> EntityManager::makeEntity(entity_type_t type) {
    std::unique_ptr<IEntityFactory>& factory = factories[type.kind];
    return factory ? factory->makeEntity(type.id) : std::shared_ptr<Entity>();
}