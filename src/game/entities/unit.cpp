//
// Created by Ion Agorria on 17/08/19
//
#include "engine/simulation/world/world.h"
#include "unit.h"

void Unit::simulationChanged() {
    if (isActive()) {
        updateAttachmentOnEntityChange = false;
        setSelectable(true);
        setBounds(Vector2(simulation->tileSize));
    }

    Entity::simulationChanged();
}

void Unit::draw() {
    ImageComponent::draw(renderer);
}