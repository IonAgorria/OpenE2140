//
// Created by Ion Agorria on 17/08/19
//
#include "engine/simulation/world/world.h"
#include "engine/entities/entity_config.h"
#include "unit.h"

void Unit::simulationChanged() {
    if (isActive()) {
        setSelectable(true);
        setBounds(Vector2(simulation->tileSize));
    }

    Entity::simulationChanged();
}

void Unit::draw() {
    ImageComponent::draw(renderer);
}