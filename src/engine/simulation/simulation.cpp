//
// Created by Ion Agorria on 1/11/18
//

#include "core/engine.h"
#include "graphics/renderer.h"
#include "entities/entity.h"
#include "enviroment/world.h"
#include "simulation.h"

Simulation::Simulation(std::shared_ptr<Engine> engine): engine(engine) {
    log = Log::get("Simulation");
    world = std::make_unique<World>("");
}

Simulation::~Simulation() {
    if (world) {
        world.reset();
    }
    //TODO dispose entities correctly
}

void Simulation::update() {
    world->update();
}

void Simulation::draw(const Rectangle& rectangle) {
    world->draw(engine->getRenderer(), rectangle);
}

const std::vector<std::unique_ptr<Entity>>& Simulation::getEntities() const {
    return entities;
}

World* Simulation::getWorld() const {
    return world.get();
}
