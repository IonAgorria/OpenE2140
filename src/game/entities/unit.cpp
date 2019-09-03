//
// Created by Ion Agorria on 17/08/19
//
#include "engine/simulation/world/world.h"
#include "unit.h"

void Unit::simulationChanged() {
    if (isActive()) {
        setSelectable(true);

        //Set bounds
        World* world = simulation->getWorld();
        bounds.setCenter(position, Vector2(world->tileSize));

        //Load image TODO
        setImageFromSprite("default_0");
    }

    Entity::simulationChanged();
}

void Unit::draw() {
    ImageComponent::draw(renderer);
}