//
// Created by Ion Agorria on 13/06/19
//
#include "building.h"

Building::Building() {
}

void Building::draw() {
}

std::shared_ptr<Entity> BuildingFactory::makeEntity(entity_type_id_t id) {
    return std::shared_ptr<Entity>();
}
