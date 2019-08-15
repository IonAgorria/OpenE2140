//
// Created by Ion Agorria on 13/06/19
//
#include "engine/io/config.h"
#include "engine/simulation/world/world.h"
#include "engine/simulation/entities/entity_config.h"
#include "building.h"

void Building::simulationChanged() {
    if (!isActive()) {
        return;
    }
    World* world = simulation->getWorld();

    //Set the building bounds
    world->toWorldRectangle(config->bounds, bounds);
    bounds += Rectangle(position, Vector2());


    //Set position to the center of bounds
    bounds.getCenter(position);

    //Layout setup to claim tiles
    config_data_t layout = config->getData("layout");
    if (layout.is_array()) {
        for (config_data_t& layoutData : layout) {
            Rectangle layoutRectangle;
            if (!Config::getRectangle(layoutData, layoutRectangle)) {
                continue;
            }
            //TODO
        }
    }

    //Set the rest
    chooseSprite();
}

void Building::chooseSprite() {
    //Load default sprites
    //TODO if (health)
    //setImageFromSprite("damaged");
    setImageFromSprite("default");
}

void Building::draw() {
    ImageComponent::draw(renderer);
}
