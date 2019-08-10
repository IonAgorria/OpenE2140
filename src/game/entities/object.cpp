//
// Created by Ion Agorria on 13/06/19
//
#include "engine/simulation/world/world.h"
#include "object.h"

void Object::simulationChanged() {
    if (!isActive()) {
        return;
    }
    World* world = simulation->getWorld();
    bounds.setCenter(position, Vector2(world->tileSize));
    setImageFromSprite("default");
}

void Object::draw() {
    ImageComponent::draw(renderer);
}