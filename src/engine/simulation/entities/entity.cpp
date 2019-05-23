//
// Created by Ion Agorria on 2018/06/03
//
#include "entity.h"
#include "simulation/simulation.h"
#include "core/utils.h"

Entity::Entity() {
}

Entity::~Entity() {
    if (id || simulation) {
        Log::get("Simulation")->warn("Entity was destructed without being removed from simulation");
        removedFromSimulation();
    }
}

std::string Entity::toString() const {
    return type_name + "(" + toStringContent() + ")";
}

std::string Entity::toStringContent() const {
    return " Position: " + position.toString()
        ;
}

const Vector2& Entity::getPosition() {
    return position;
}

void Entity::addedToSimulation(Simulation* sim) {
    simulation = sim;
    id = simulation->nextEntityID();
}

void Entity::removedFromSimulation() {
    simulation = nullptr;
    id = 0;
}

Image* Entity::draw(Vector2& drawPosition, Vector2& drawSize, float& drawAngle, Palette* palette) {
    return nullptr;
}

