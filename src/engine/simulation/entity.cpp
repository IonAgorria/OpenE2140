//
// Created by Ion Agorria on 2018/06/03
//
#include "engine/core/utils.h"
#include "engine/simulation/simulation.h"
#include "entity.h"

Entity::Entity() {
}

Entity::~Entity() {
    if (active || id || simulation) {
        Log::get("Simulation")->warn("Entity {0} was destructed without being removed from simulation", toString());
    }
}

void Entity::setup(EntityConfig* newConfig) {
    config = newConfig;
    componentsSetup();
}

entity_id_t Entity::getID() const {
    return id;
}

const Vector2& Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const Vector2& newPosition) {
    position.set(newPosition);
}

entity_direction_t Entity::getDirection() const {
    return direction;
}

void Entity::setDirection(entity_direction_t newDirection) {
    direction = newDirection;
}

const Rectangle& Entity::getBounds() const {
    return bounds;
}

void Entity::setBounds(const Rectangle& newBounds) {
    bounds.set(newBounds);
}

const EntityConfig* Entity::getConfig() const {
    return config;
}

Simulation* Entity::getSimulation() const {
    return simulation;
}

void Entity::setParent(Entity* entity) {
    parent = entity;
}

Entity* Entity::getParent() const {
    return parent;
}

void Entity::addedToSimulation(entity_id_t entityID, Simulation* sim) {
    id = entityID;
    simulation = sim;
    renderer = simulation->getRenderer();
    active = true;
    simulationChanged();
}

void Entity::removedFromSimulation() {
    active = false;
    simulationChanged();
    renderer = nullptr;
    simulation = nullptr;
    id = 0;
}

void Entity::simulationChanged() {
    componentsSimulationChanged();
}

void Entity::update() {
    componentsUpdate();
}

void Entity::draw() {
}

bool Entity::isActive() {
    return active;
}

bool Entity::isDisable() const {
    return disable;
}

void Entity::setDisable(bool newDisable) {
    disable = newDisable;
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
    return "ID: " + std::to_string(id) +
           " Active: " + std::to_string(active) +
           (parent ? " Parent: " + std::to_string(parent->id) : "") +
           " Position: " + position.toString() +
           "";
}
