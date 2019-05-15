//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_SIMULATION_H
#define OPENE2140_SIMULATION_H

#include "core/common.h"

class Engine;
class World;
class Entity;
class Renderer;

/**
 * Contains everything inside the running game
 */
class Simulation {
private:
    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Engine object
     */
    std::shared_ptr<Engine> engine;

    /**
     * Entities contained by this simulation
     */
    std::vector<std::unique_ptr<Entity>> entities;

    /**
     * World for this simulation
     */
    std::unique_ptr<World> world;

public:
    /**
     * Constructor
     */
    Simulation(std::shared_ptr<Engine> engine);

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
    const std::vector<std::unique_ptr<Entity>>& getEntities() const;

    /**
     * @return World instance in simulation
     */
    World* getWorld() const;
};

#endif //OPENE2140_SIMULATION_H
