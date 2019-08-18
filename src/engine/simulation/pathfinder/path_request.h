//
// Created by Ion Agorria on 14/06/19
//
#ifndef OPENE2140_PATH_REQUEST_H
#define OPENE2140_PATH_REQUEST_H

#include <map>
#include <optional>
#include "engine/core/macros.h"
#include "engine/math/vector2.h"
#include "astar.h"

class Tile;
class Entity;
class Simulation;

/**
 * Contains the request for pathfinder, can contain one or several agents that want go to a single fixed destination or
 * follow another moving agent. Stores the common reusable vertex data if the goal is the same.
 */
class PathRequest {
protected:
    /**
     * Simulation which request was created for
     */
    Simulation* simulation;

    /**
     * Pathfinder assigned to each agent
     */
    std::map<entity_id_t, std::unique_ptr<AStar>> pathfinders;

    /**
     * Destination for this request
     */
    Tile* destination;

    /**
     * Target agent for this request if any
     */
    std::shared_ptr<Entity> target;

    /**
     * Stores vertexes for state keeping, also manages the ownership of vertex memory
     */
    std::vector<PathVertex> vertexes;

public:
    /**
     * Constructor
     */
    PathRequest();

    /**
     * Destructor
     */
    ~PathRequest() = default;

    /**
     * (re)initializes the internal states to clear state
     */
    void initialize();

    /**
     * @return the common vertexes for this request
     */
    std::vector<PathVertex>& getVertexes();

    /**
     * Adds a new entity to this request
     *
     * @param entity
     * @return if was added
     */
    bool addEntity(entity_id_t entity);

    /**
     * Removes a entity from this request
     *
     * @param entity
     * @return if was removed
     */
    bool removeEntity(entity_id_t entity);

    /**
     * Returns the current status of pathfinder for the provided entity
     * If path is available the provided vector will filled
     *
     * @param entity which requested the path
     * @param path vector to write path if available
     * @return path status
     */
    PathFinderStatus getResult(entity_id_t entity, std::vector<tile_index_t> path);

    /**
     * Sets the destination if is different from current destination
     *
     * @param newDestination
     */
    void setDestination(Tile* newDestination);

    /**
     * Sets target entity as destination
     *
     * @param entity
     */
    void setTarget(std::shared_ptr<Entity> entity);

    /**
     * @return true if all entities finished
     */
    bool empty();

    /**
     * Updates the entities and pathfinder stuff
     */
    void update();
};

#endif //OPENE2140_PATH_REQUEST_H
