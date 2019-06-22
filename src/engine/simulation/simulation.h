//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_SIMULATION_H
#define OPENE2140_SIMULATION_H

#include "engine/core/common.h"
#include "entity.h"
#include "simulation_parameters.h"
#include "engine/assets/asset_manager.h"
#include "engine/io/log.h"

class Engine;
class World;
class Renderer;

/**
 * Contains everything inside the running game
 */
class Simulation: public IErrorPossible {
private:
    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Simulation setup parameters
     */
    std::unique_ptr<SimulationParameters> parameters;

    /**
     * Engine object
     */
    std::shared_ptr<Engine> engine;

    /**
     * Entities contained by this simulation
     */
    std::vector<std::shared_ptr<Entity>> entities;

    /**
     * World for this simulation
     */
    std::unique_ptr<World> world;

    /**
     * Last used entity id
     */
    entity_id_t lastEntityID = 0;

public:
    /**
     * Constructor
     */
    Simulation(std::shared_ptr<Engine> engine, std::unique_ptr<SimulationParameters>& parameters);

    /**
     * Destructor
     */
    virtual ~Simulation();

    /**
     * Disable copy
     */
    NON_COPYABLE(Simulation)

    /**
     * Called when simulation is being updated
     */
    virtual void update();

    /**
     * Draws this simulation
     *
     * @param rectangle the rectangle to draw
     */
    virtual void draw(const Rectangle& rectangle);

    /**
     * @return entities in simulation
     */
    const std::vector<std::shared_ptr<Entity>>& getEntities() const;

    /**
     * @return World instance in simulation
     */
    World* getWorld() const;

    /**
     * @return the next entity ID
     */
    entity_id_t nextEntityID();

    /**
     * Adds entity to simulation
     */
    void addEntity(std::shared_ptr<Entity> entity);

    /**
     * Removes entity from simulation
     */
    void removeEntity(std::shared_ptr<Entity> entity);

    /*
     * AssetManager proxy
     */

    /**
     * Gets the loaded image from an asset
     *
     * @return image
     */
    std::shared_ptr<Image> getImage(const asset_path_t& path);
};

#endif //OPENE2140_SIMULATION_H
