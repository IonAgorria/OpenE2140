//
// Created by Ion Agorria on 1/11/18
//

#include "engine/core/utils.h"
#include "core/engine.h"
#include "graphics/renderer.h"
#include "entities/entity.h"
#include "enviroment/world.h"
#include "assets/asset.h"
#include "assets/asset_manager.h"
#include "simulation.h"

Simulation::Simulation(std::shared_ptr<Engine> engine, std::unique_ptr<SimulationParameters> parameters):
        engine(engine), parameters(std::move(parameters)) {
    log = Log::get("Simulation");
    world = std::make_unique<World>();
    Asset* asset = engine->getAssetManager()->getAsset(parameters->world);
    if (!asset) {
        error = "World asset not found";
        return;
    }
}

Simulation::~Simulation() {
    log->debug("Closing");
    for (std::shared_ptr<Entity>& entity : entities) {
        entity->removedFromSimulation();
    }
    entities.clear();
    if (world) {
        world.reset();
    }
}

void Simulation::update() {
    world->update();
}

void Simulation::draw(const Rectangle& rectangle) {
    Renderer* renderer = engine->getRenderer();
    world->draw(renderer, rectangle);

    for (std::shared_ptr<Entity>& entity : entities) {
        //entity->updatePalette();
        Image* image = entity->getImage();
    }
}

const std::vector<std::shared_ptr<Entity>>& Simulation::getEntities() const {
    return entities;
}

World* Simulation::getWorld() const {
    return world.get();
}

entity_id Simulation::nextEntityID() {
    lastEntityID++;
    return lastEntityID;
}

void Simulation::addEntity(std::shared_ptr<Entity> entity) {
    entities.emplace_back(std::move(entity));
    entity->addedToSimulation(this);
}

void Simulation::removeEntity(std::shared_ptr<Entity> entity) {
    Utils::eraseElementFromVector(entities, entity);
    entity->removedFromSimulation();
}
