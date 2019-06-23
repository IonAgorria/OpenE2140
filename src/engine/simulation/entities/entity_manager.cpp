//
// Created by Ion Agorria on 23/06/19
//

#include "entity_manager.h"

EntityManager::EntityManager(std::shared_ptr<Engine> engine): engine(engine) {
    log = Log::get("Entities");
}

EntityManager::~EntityManager() {
    log->debug("Closing");
    clearAssets();
    factories.clear();
}

void EntityManager::addEntityFactory(std::unique_ptr<IEntityFactory> factory) {
    factory->setManager(this);
    factories.push_back(std::move(factory));
}
