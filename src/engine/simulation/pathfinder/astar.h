//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_ASTAR_H
#define OPENE2140_ASTAR_H

#include <unordered_map>
#include <memory>

#include "engine/core/macros.h"
#include "engine/core/priority_queue.h"
#include "path_state.h"
#include "path_vertex.h"
#include "astar_comparator.h"

class PathRequest;
class Tile;

/**
 * A* based pathfinder implementation
 */
class AStar {
protected:
    /**
     * The path request which spawned this pathfinder
     */
    PathRequest* request;

    /**
     * Current pathfinder state
     */
    PathFinderStatus status = PathFinderStatus::None;

    /**
     * The tile where the agent are is sitting
     */
    Tile* start = nullptr;

    /**
     * The tile which is desired to reach
     */
    Tile* goal = nullptr;

    /**
     * Priority queue of vertexes or open list
     */
    PriorityQueue<PathVertex*, AStarComparator> queue;

public:
    /**
     * Calculated heuristic cost of each tile
     */
    std::vector<path_cost_t> heuristic;

    /**
     * Constructor
     */
    AStar(PathRequest* request);

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
     * Tells the pathfinder to visit the vertex
     *
     * @param vertex to visit
     */
    void visitTile(World* world, PathVertex& vertex, PathVertex* from);
};

#endif //OPENE2140_ASTAR_H
