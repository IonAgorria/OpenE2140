//
// Created by Ion Agorria on 14/06/19
//
#ifndef OPENE2140_PATH_REQUEST_H
#define OPENE2140_PATH_REQUEST_H

#include <src/engine/core/types.h>
#include "math/vector2.h"
#include "astar.h"

class Tile;
class Entity;

/**
 * Contains the request for pathfinder, can contain one or several agents that want go to a single fixed destination or
 * follow another moving agent. Stores the common reusable vertex data if the goal is the same.
 */
class PathRequest {
protected:
    /**
     * AStar pathfinder assigned to each agent
     */
    std::unordered_map<entity_id_t, std::unique_ptr<AStar>> astar;

    /**
     * Destination for this request
     */
    Vector2 destination;

    /**
     * Target agent for this request if any
     */
    std::shared_ptr<Entity> target;

    /**
     * Stores map of Tile <-> Vertex for state keeping, also manages the ownership of vertex memory
     */
    std::vector<std::unique_ptr<PathVertex>> vertexes;
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
     * Returns the common vertex for this request
     *
     * @param tile index of tile
     * @param from the vertex which is coming or null if none
     * @return vertex
     */
    PathVertex* getVertex(tile_index_t tile, PathVertex* from);
};

#endif //OPENE2140_PATH_REQUEST_H
