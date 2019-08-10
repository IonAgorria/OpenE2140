//
// Created by Ion Agorria on 13/06/19
//
#include "engine/io/config.h"
#include "engine/simulation/world/world.h"
#include "building.h"

void Building::simulationChanged() {
    if (!isActive()) {
        return;
    }

    World* world = simulation->getWorld();
    world->toWorldRectangle(config->bounds, bounds);
    bounds += Rectangle(position, Vector2());
    chooseSprite();
}

void Building::chooseSprite() {
    //Load default sprites
    //TODO if (health)
    //setAnimationFromSprite("damaged");
    setAnimationFromSprite("default");
}

void Building::draw() {
    AnimationComponent::draw(renderer);
}

void BuildingFactory::setupEntityConfig(EntityConfig* config) {
    //Setup the bounds from layout of building if bounds is not set in config and layout exists
    config_data_t layout = config->getData("layout");
    if (!config->getData("bounds").is_array() && layout.is_array()) {
        Rectangle bounds;
        //Do an union of each rectangle
        for (config_data_t& layoutData : layout) {
            Rectangle layoutRectangle;
            if (Config::getRectangle(layoutData, layoutRectangle)) {
                bounds.getUnionRectangle(layoutRectangle, bounds);
            }
        }
        config_data_t boundsData;
        Config::setRectangle(bounds, boundsData);
        config->setData("bounds", boundsData);
        config->bounds = bounds;
    }

    IEntityFactory::setupEntityConfig(config);
}
