//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_ASTAR_H
#define OPENE2140_ASTAR_H

#include <unordered_map>
#include <memory>

#include "core/common.h"
#include "core/priority_queue.h"
#include "path_state.h"
#include "path_vertex.h"

using path_cost_t = unsigned short;

class Tile;

/**
 * Custom priority queue comparator
 */
class AStarComparator {
public:
    bool operator()(PathVertex& v1, PathVertex& v2)
    {
        path_cost_t f1 = v1.g;// + h1;
        path_cost_t f2 = v2.g;// + h2;
        return f1 > f2;
    }
};

/**
 * A* based pathfinder implementation
 */
class AStar {
protected:
    /**
     * Current pathfinder state
     */
    PathFinderStatus status;

    /**
     * The tile where the agent are is sitting
     */
    Tile* start;

    /**
     * The tile which is desired to reach
     */
    Tile* goal;

    /**
     * Priority queue of vertexes or open list
     */
    PriorityQueue<PathVertex*> queue;
public:

    /**
     * Constructor
     */
    AStar();

    /**
     * Destructor
     */
    ~AStar() = default;

    /**
     * (re)initializes the internal states to clear state
     */
    void initialize();

    /**
     * Notifies the pathfinder about the current desired plan
     *
     * @param newStarts
     * @param newGoal
     */
    void plan(Tile* newStart, Tile* newGoal);

    /**
     * Does the main computation of algorithm
     */
    void compute();

    /**
     * @return current path finder status
     */
    PathFinderStatus getStatus();

    /**
     * Tells the pathfinder to visit the tile
     *
     * @param tile to visit
     */
    void visitTile(Tile* tile, PathVertex* from);
};

#endif //OPENE2140_ASTAR_H
