//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_SIMULATION_H
#define OPENE2140_SIMULATION_H

#include "engine/core/common.h"
#include "engine/simulation/entities/entity.h"
#include "simulation_parameters.h"
#include "engine/assets/asset_manager.h"
#include "engine/io/log.h"

class EntityPrototype;
class Faction;
class Player;
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
     * Factions for this simulation
     */
    std::vector<std::unique_ptr<Faction>> factions;

    /**
     * Players for this simulation
     */
    std::vector<std::unique_ptr<Player>> players;

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
     * Called when simulation should load from parameters
     */
    virtual void load();

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
     * Creates a new entity and adds to simulation
     */
    void createEntity(const EntityPrototype& entityPrototype);

    /**
     * Creates a new entity and adds to simulation
     */
    void createEntity(const entity_type_t& entityType);

    /**
     * Adds entity to simulation
     */
    void addEntity(std::shared_ptr<Entity> entity);

    /**
     * Removes entity from simulation
     */
    void removeEntity(std::shared_ptr<Entity> entity);

    /**
     * @return current renderer
     */
    Renderer* getRenderer() const;

    /**
     * Adds a new faction to simulation
     *
     * @param faction to add
     */
    void addFaction(std::unique_ptr<Faction> faction);

    /**
     * Obtain the faction from ID
     *
     * @param id of faction
     * @return faction or null if none found
     */
    Faction* getFaction(faction_id_t id) const;

    /**
     * Obtain the faction from code
     *
     * @param code of faction
     * @return faction or null if none found
     */
    Faction* getFaction(const std::string& code) const;

    /**
     * Adds a new player to simulation
     *
     * @param player to add
     */
    void addPlayer(std::unique_ptr<Player> player);

    /**
     * Obtain the player from ID
     *
     * @param id of player
     * @return player or null if none found
     */
    Player* getPlayer(player_id_t id) const;

    /*
     * AssetManager proxy
     */

    /**
     * Gets the loaded image from an asset
     *
     * @return image
     */
    Image* getImage(const asset_path_t& path) const;
};

#endif //OPENE2140_SIMULATION_H
