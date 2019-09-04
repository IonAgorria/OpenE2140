//
// Created by Ion Agorria on 13/06/19
//
#include "engine/simulation/world/world.h"
#include "object.h"

void Object::simulationChanged() {
    if (isActive()) {
        //Set bounds
        World* world = simulation->getWorld();
        setBounds(Vector2(world->tileSize));

        //Load image according to tileset
        setImageFromSprite("default_" + std::to_string(world->tilesetIndex));
    }

    Entity::simulationChanged();
}

void Object::draw() {
    ImageComponent::draw(renderer);
}