//
// Created by Ion Agorria on 1/11/18
//

#include "engine/core/utils.h"
#include "core/engine.h"
#include "graphics/renderer.h"
#include "entity.h"
#include "world.h"
#include "assets/asset.h"
#include "assets/asset_level.h"
#include "assets/asset_manager.h"
#include "simulation.h"

void CompA::construction() {
}
void CompB::construction() {
}
void CompC::construction() {
}
CompA::~CompA() {
}
CompB::~CompB() {
}
CompC::~CompC() {
}
void CompA::simulationChanged() {
}
void CompB::simulationChanged() {
}
void CompC::simulationChanged() {
}
void CompA::update() {
    Log::get()->debug("CALLED A "+type_name() + " BASE "+ (base->type_name()));
    CompB* b = castBase<CompB>();
    if (b) Log::get()->debug("GOT "+b->type_name());
}
void CompB::update() {
    Log::get()->debug("CALLED B "+type_name() + " BASE "+ (base->type_name()));
}
void CompC::update() {
    Log::get()->debug("CALLED C "+type_name());
}

Simulation::Simulation(std::shared_ptr<Engine> engine, std::unique_ptr<SimulationParameters>& parameters):
        parameters(std::move(parameters)), engine(engine) {
    log = Log::get("Simulation");
    if (!this->parameters || this->parameters->world.empty()) {
        error = "Parameters not set";
        return;
    }

    //Load asset
    AssetLevel* assetLevel = engine->getAssetManager()->getAsset<AssetLevel>(this->parameters->world);
    if (!assetLevel) {
        error = "World asset not found";
        return;
    }
    log->debug("Name: '" + assetLevel->name() + "'");

    //Load tileset
    std::unordered_map<unsigned int, std::shared_ptr<Image>> tilesetImages;
    size_t tilesetSize = assetLevel->tilesetSize();
    for (size_t i = 0; i < tilesetSize; ++i) {
        asset_path_t path = assetLevel->tilePath(i);
        if (!path.empty()) {
            tilesetImages[i] = getImage(path);
        }
    }
    error = assetLevel->getError();
    if (hasError()) {
        return;
    }

    //Create world
    world = std::make_unique<World>(assetLevel, tilesetImages);
    error = world->getError();
    if (hasError()) {
        return;
    }

    //Load players
    std::vector<PlayerPrototype> levelPlayers;
    assetLevel->players(levelPlayers);
    error = assetLevel->getError();
    if (hasError()) {
        return;
    }

    //Load entities
    std::vector<EntityPrototype> levelEntities;
    assetLevel->entities(levelEntities);
    error = assetLevel->getError();
    if (hasError()) {
        return;
    }

    entities.emplace_back(std::move(std::make_shared<EntA>()));
    entities.emplace_back(std::move(std::make_shared<EntB>()));
    entities.emplace_back(std::move(std::make_shared<EntC>()));
    entities.emplace_back(std::move(std::make_shared<EntD>()));
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
    for (std::shared_ptr<Entity>& entity : entities) {
        entity->update();
    }
}

void Simulation::draw(const Rectangle& rectangle) {
    Renderer* renderer = engine->getRenderer();
    world->draw(renderer, rectangle);

    //Draw entities
    Vector2 position;
    Vector2 size;
    Rectangle imageRectangle;
    float angle = 0;
    Palette* extraPalette = nullptr;
    for (std::shared_ptr<Entity>& entity : entities) {
        Image* image = entity->draw(position, size, angle, extraPalette);
        imageRectangle.setCenter(position, size);
        if (image && rectangle.isOverlap(imageRectangle)) {
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

std::shared_ptr<Image> Simulation::getImage(const asset_path_t& path) {
    return engine->getAssetManager()->getImage(path);
}
