//
// Created by Ion Agorria on 13/06/19
//
#include "engine/io/config.h"
#include "engine/simulation/world/world.h"
#include "src/engine/entities/entity_config.h"
#include "building.h"

void Building::simulationChanged() {
    if (isActive()) {
        setSelectable(true);

        //Set the building bounds
        simulation->toWorldRectangle(config->bounds, bounds, false);
        bounds += Rectangle(position);

        //Set position to the center of bounds
        bounds.getCenter(position);

        //Apply offset after setting center
        Rectangle offset;
        config->getRectangle("offset", offset);
        bounds += offset;

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
    }

    Entity::simulationChanged();
}

void Building::draw() {
    ImageComponent::draw(renderer);
}
