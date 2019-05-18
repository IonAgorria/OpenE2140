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
    return "Entity(" + toStringContent() + ")";
}

std::string Entity::toStringContent() const {
    return " Rectangle: " + rectangle.toString()
        ;
}

void Entity::addedToSimulation(Simulation* sim) {
    simulation = sim;
    id = simulation->nextEntityID();
}

void Entity::removedFromSimulation() {
    simulation = nullptr;
    id = 0;
}

