//
// Created by Ion Agorria on 29/06/19
//

#include "entity.h"
#include "entity_factory.h"


void IEntityFactory::clear() {
}

void IEntityFactory::load() {
}

void IEntityFactory::setManager(EntityManager* current) {
    manager = current;
}

std::shared_ptr<Entity> IEntityFactory::makeEntity(entity_type_id_t id) {
    std::shared_ptr<Entity> entity = instanceEntity(id);
    //Only setup if entity was instanced
    if (entity) {
        EntityConfig* config = nullptr;
        if (id < configs.size()) {
            config = configs[id].get();
        }
        if (config) {
            entity->setup(config);
        } else {
            error = "Entity config not found for id: " + std::to_string(id);
        }
    }
    return entity;
}