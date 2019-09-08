//
// Created by Ion Agorria on 2018/06/03
//
#include "engine/simulation/simulation.h"
#include "engine/entities/entity_config.h"
#include "entity.h"

Entity::Entity() {
}

Entity::~Entity() {
    if (active || id || simulation) {
        BUG("Entity {0} was destructed without being removed from simulation", toString());
    }
}

void Entity::setup(EntityConfig* newConfig) {
    config = newConfig;
    if (config) {
        maxHealth = config->health;
        currentHealth = config->health;
    }
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
    bounds.setCenter(newPosition);
    changesCount++;
}

entity_direction_t Entity::getDirection() const {
    return direction;
}

void Entity::setDirection(entity_direction_t newDirection) {
    direction = newDirection;
    changesCount++;
}

const Rectangle& Entity::getBounds() const {
    return bounds;
}

void Entity::setBounds(const Vector2& newBounds) {
    bounds.setCenter(position, newBounds);
    changesCount++;
}

void Entity::setMaxHealth(entity_health_t newHealth) {
    if (newHealth < currentHealth) currentHealth = newHealth;
    maxHealth = newHealth;
}

entity_health_t Entity::getMaxHealth() {
    return maxHealth;
}

void Entity::setCurrentHealth(entity_health_t newHealth) {
    if (maxHealth < newHealth) newHealth = maxHealth;
    currentHealth = newHealth;
}

entity_health_t Entity::getCurrentHealth() {
    return currentHealth;
}

void Entity::addHealth(entity_health_t health) {
    if (0 < health) {
        setCurrentHealth(currentHealth + health);
    }
}

void Entity::removeHealth(entity_health_t health) {
    if (0 < health) {
        setCurrentHealth(currentHealth - health);
    }
}

bool Entity::hasHealth() {
    return 0 < maxHealth;
}

bool Entity::isDamaged() {
    return 0 < maxHealth && currentHealth < maxHealth;
}

bool Entity::isDestroyed() {
    return 0 < maxHealth && 0 == currentHealth;
}

const EntityConfig* Entity::getConfig() const {
    return config;
}

Simulation* Entity::getSimulation() const {
    return simulation;
}

void Entity::setParent(Entity* entity) {
    parent = entity;
    changesCount++;
}

Entity* Entity::getParent() const {
    return parent;
}

void Entity::addedToSimulation(entity_id_t entityID, Simulation* sim) {
    id = entityID;
    simulation = sim;
    renderer = simulation->getRenderer();
    active = true;
    bounds.set(config->bounds);
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

bool Entity::isActive() const {
    return active;
}

bool Entity::isDisable() const {
    return disable;
}

void Entity::setDisable(bool newDisable) {
    disable = newDisable;
    changesCount++;
}

bool Entity::isSelectable() const {
    return selectable;
}

void Entity::setSelectable(bool newSelectable) {
    selectable = newSelectable;
    changesCount++;
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
           " Health: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth) + " "
           "";
}
