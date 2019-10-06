//
// Created by Ion Agorria on 13/06/19
//
#include "engine/io/config.h"
#include "engine/simulation/world/world.h"
#include "src/engine/entities/entity_config.h"
#include "attachment.h"
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

void Building::entityChanged() {
    setLight(energySatisfied);

    //First call parent code
    Entity::entityChanged();
}

void Building::draw() {
    ImageComponent::draw(renderer);
}

void Mine::simulationChanged() {
    //First call parent code
    Building::simulationChanged();

    if (isActive()) {
        //Setup attached entities
        Spinner* entity = dynamic_cast<Spinner*>(AttachmentComponent::getAttached("flywheel_bottom").get());
        if (entity) {
            entity->clockwise = false;
        }
    }
}

void Mine::entityChanged() {
    //Update attached entities energy
    Spinner* spinner = dynamic_cast<Spinner*>(AttachmentComponent::getAttached("flywheel_top").get());
    if (spinner) {
        spinner->animationPlay = energySatisfied;
    }
    spinner = dynamic_cast<Spinner*>(AttachmentComponent::getAttached("flywheel_bottom").get());
    if (spinner) {
        spinner->animationPlay = energySatisfied;
    }

    //First call parent code
    Entity::entityChanged();
}

void Refinery::simulationChanged() {
    //First call parent code
    Building::simulationChanged();

    if (isActive()) {
        //Setup attached entities
        ConveyorBelt* entity = dynamic_cast<ConveyorBelt*>(AttachmentComponent::getAttached("conveyor_belt").get());
        if (entity) {
            entity->setDirection(true);
        }
    }
}

void Refinery::entityChanged() {
    /*
    ConveyorBelt* conveyor = dynamic_cast<ConveyorBelt*>(AttachmentComponent::getAttached("conveyor_belt").get());
    if (conveyor) {
        conveyor->setRunning();
    }
    */

    //First call parent code
    Building::entityChanged();
}