//
// Created by Ion Agorria on 2018/06/03
//
#include "core/utils.h"
#include "simulation/simulation.h"
#include "entity.h"

Entity::Entity() {
}

Entity::~Entity() {
    if (id || simulation) {
        Log::get("Simulation")->warn("Entity was destructed without being removed from simulation");
        removedFromSimulation();
    }
}

std::string Entity::toString() const {
    return type_name() + "(" + toStringContent() + ")";
}

std::string Entity::toStringContent() const {
    return " ID: " + std::to_string(id) +
           " Position: " + position.toString()
   ;
}

const Vector2& Entity::getPosition() {
    return position;
}

void Entity::addedToSimulation(Simulation* sim) {
    simulation = sim;
    id = simulation->nextEntityID();
    componentsSimulationChanged();
}

void Entity::removedFromSimulation() {
    componentsSimulationChanged();
    simulation = nullptr;
    id = 0;
}

void Entity::update() {
    componentsUpdate();
}

Image* Entity::draw(Vector2& drawPosition, Vector2& drawSize, float& drawAngle, Palette* palette) {
    return nullptr;
}

bool Entity::active() {
    return id != 0 && simulation != nullptr;
}

