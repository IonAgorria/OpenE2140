//
// Created by Ion Agorria on 2018/06/03
//
#include "engine/core/utils.h"
#include "engine/simulation/simulation.h"
#include "entity.h"

Entity::Entity() {
}

Entity::~Entity() {
    if (id || simulation) {
        Log::get("Simulation")->warn("Entity was destructed without being removed from simulation");
        removedFromSimulation();
    }
}

void Entity::setup(EntityConfig* newConfig) {
    config = newConfig;
}

const Vector2& Entity::getPosition() const {
    return position;
}

const Rectangle& Entity::getBounds() const {
    return bounds;
}

const EntityConfig* Entity::getConfig() const {
    return config;
}

Simulation* Entity::getSimulation() const {
    return simulation;
}

void Entity::addedToSimulation(Simulation* sim) {
    simulation = sim;
    renderer = simulation->getRenderer();
    id = simulation->nextEntityID();
    active = true;
    componentsSimulationChanged();
}

void Entity::removedFromSimulation() {
    active = false;
    componentsSimulationChanged();
    renderer = nullptr;
    simulation = nullptr;
    id = 0;
}

void Entity::update() {
    componentsUpdate();
}

void Entity::draw() {
}

bool Entity::isActive() {
    return active;
}

Tile* Entity::getTile() {
    return tiles.empty() ? nullptr : tiles[0];
}

const std::vector<Tile*>& Entity::getTiles() {
    return tiles;
}

std::string Entity::toString() const {
    return type_name() + "(" + toStringContent() + ")";
}

std::string Entity::toStringContent() const {
    return " ID: " + std::to_string(id) +
           " Position: " + position.toString()
            ;
}