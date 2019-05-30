//
// Created by Ion Agorria on 1/11/18
//

#include "engine/core/utils.h"
#include "core/engine.h"
#include "graphics/renderer.h"
#include "entities/entity.h"
#include "enviroment/world.h"
#include "assets/asset.h"
#include "assets/asset_world.h"
#include "assets/asset_manager.h"
#include "simulation.h"

Simulation::Simulation(std::shared_ptr<Engine> engine, std::unique_ptr<SimulationParameters>& parameters):
        parameters(std::move(parameters)), engine(engine) {
    log = Log::get("Simulation");
    if (!parameters || parameters->world.empty()) {
        error = "Parameters not set";
    }
    AssetWorld* assetWorld = engine->getAssetManager()->getAsset<AssetWorld>(this->parameters->world);
    if (!assetWorld) {
        error = "World asset not found";
        return;
    }
    world = std::make_unique<World>(assetWorld);
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

    //Draw entities
    Vector2 position;
    Vector2 size;
    float angle = 0;
    Palette* extraPalette = nullptr;
    for (std::shared_ptr<Entity>& entity : entities) {
        Image* image = entity->draw(position, size, angle, extraPalette);
        if (image) {
            renderer->draw(position.x, position.y, size.x, size.y, angle, *image, extraPalette);
        }
    }
}

const std::vector<std::shared_ptr<Entity>>& Simulation::getEntities() const {
    return entities;
}

World* Simulation::getWorld() const {
    return world.get();
}

entity_id_t Simulation::nextEntityID() {
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
