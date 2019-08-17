//
// Created by Ion Agorria on 27/04/19
//
#ifndef OPENE2140_GAME_H
#define OPENE2140_GAME_H

#include "engine/core/engine.h"

class Tile;

/**
 * Contain's game specific code
 */
class Game: public Engine {
protected:
    void run() override;

    void setupEventHandler() override;

    void setupAssetManager() override;

    void setupEntityManager() override;

    void Engine::setupSimulation(std::unique_ptr<SimulationParameters> parameters) override;

    void setupPlayerColors(Player& player);

    /**
     * Sets a tile as reactor crate
     * TODO move this to some world or powerplant specific class
     */
    void setReactorCrate(Tile& tile);
};

#endif //OPENE2140_GAME_H
