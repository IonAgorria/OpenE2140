//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_ASTAR_H
#define OPENE2140_ASTAR_H

#define ASTAR_MAX_STEPS 1000

#include <unordered_map>
#include <memory>

#include "engine/core/macros.h"
#include "engine/core/priority_queue.h"
#include "path_state.h"
#include "path_vertex.h"
#include "astar_comparator.h"

class World;
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
     * The closest tile to target
     */
    Tile* closest = nullptr;

    /**
     * Priority queue of vertexes or open list
     */
    PriorityQueue<PathVertex*, AStarComparator> queue;

    /**
     * Flags of tiles that should be present to be passable
     */
    tile_flags_t tileFlagsRequired = 0;

    /**
     * Entity flags in tiles that shouldn't be set
     */
    tile_flags_t entityFlagsMask = 0;

    /**
     * Index of tile where entity is located, this allows not considering a tile as occupied by itself
     */
    tile_index_t entityTileIndex;

    /**
     * Tells the pathfinder to visit the vertex
     *
     * @param world pointer of world containing tiles
     * @param vertexes the vertexes containing vector
     * @param vertex to visit
     */
    void visitTile(const World* world, std::vector<PathVertex>& vertexes, PathVertex& vertex);

    /**
     * Updates the heuristic value of vertex
     *
     * @param tile
     */
    void calculateHeuristic(Tile* tile);

public:
    /**
     * Calculated heuristic cost of each tile
     */
    std::vector<path_cost_t> heuristic;

    /**
     * Constructor
     */
    AStar(PathRequest* request, tile_flags_t tileFlagsRequired);

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
     * @param newTileFlags
     * @param entityTileIndex
     */
    void plan(Tile* newStart, Tile* newGoal, tile_flags_t newTileFlags, tile_index_t newEntityTileIndex);

    /**
     * Does the main computation of algorithm
     */
    void compute();

    /**
     * @return current path finder status
     */
    PathFinderStatus getStatus();

    /**
     * @return closest found tile
     */
    Tile* getClosest();

    /**
     * Checks if provided vertex is stale (outdated state or never visited)
     *
     * @param vertex
     * @param tile
     * @return true if outdated state
     */
    static bool staleVertex(PathVertex& vertex, Tile* tile);

    bool isOccupied(Tile* tile);
};

#endif //OPENE2140_ASTAR_H
