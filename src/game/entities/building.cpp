//
// Created by Ion Agorria on 13/06/19
//
#include "building.h"

Building::Building() {
}

void Building::draw() {
}

/*
 * BuildingFactory stuff
 */

std::shared_ptr<Entity> BuildingFactory::instanceEntity(entity_type_id_t id) {
    //TODO load the config for buildings
    std::shared_ptr<Building> building;
    building = std::make_shared<Building>();

    return building;
}
