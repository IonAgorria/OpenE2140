//
// Created by Ion Agorria on 13/06/19
//
#include "engine/simulation/world/world.h"
#include "astar.h"
#include "path_request.h"

AStar::AStar(PathRequest* request, tile_flags_t tileFlagsRequired):
request(request),
tileFlagsRequired(tileFlagsRequired) {
    queue.getComparator().astar = this;
}

void AStar::initialize() {
    queue.clear();
    heuristic.clear();
    heuristic.resize(request->getVertexes().size(), PATHFINDER_INFINITY);
}

void AStar::plan(Tile* newStart, Tile* newGoal, tile_flags_t newEntityFlagsMask) {
    if (status == PathFinderStatus::Computing
        && start == newStart
        && goal == newGoal
        && entityFlagsMask == newEntityFlagsMask) {
        //No need to initialize
        return;
    }
    initialize();
    start = newStart;
    goal = newGoal;
    entityFlagsMask = newEntityFlagsMask;
    status = PathFinderStatus::Computing;

    //Check if by chance goal was already found previously and is not stale
    PathVertex* vertex = &request->getVertexes().at(goal->index);
    if (staleVertex(*vertex, goal)) {
        //Add start vertex to start path finding
        vertex = &request->getVertexes().at(start->index);
        calculateHeuristic(start);
        vertex->l = start->entityFlags;
        vertex->g = 0;
        vertex->back = vertex->index;
    }
    queue.push(vertex);
}

void AStar::compute() {
    if (status != PathFinderStatus::Computing) {
        return;
    }
    World* world = request->getWorld();
    std::vector<PathVertex>& vertexes = request->getVertexes();
    if (!world || vertexes.empty()) {
        return;
    }

    //Get the top vertex to scan next and visit it until enough steps are done
    size_t steps = 0;
    while (!queue.empty() && steps < ASTAR_MAX_STEPS) {
        PathVertex vertex = *queue.top();
        queue.pop();
        visitTile(world, vertexes, vertex);
        steps++;
    }

    //Nothing on queue and not success?
    if (queue.empty() && status != PathFinderStatus::Success) {
        if (closest) {
            status = PathFinderStatus::Partial;
        } else {
            status = PathFinderStatus::Fail;
        }
    }
}


void AStar::visitTile(const World* world, std::vector<PathVertex>& vertexes, PathVertex& vertex) {
    tile_index_t vertexIndex = vertex.index;
    Tile* tile = world->getTile(vertexIndex);
    log_ptr log = Log::get("A* "+std::to_string(start->index)+" "+std::to_string(goal->index));
    log->debug("VISIT " + tile->toString());

    //Add as closest if it's the case
    if (!closest || heuristic[closest->index] > heuristic[vertexIndex]) {
        closest = tile;
    }

    //Check if it's the goal
    if (vertexIndex == goal->index) {
        status = PathFinderStatus::Success;
        queue.clear();
        log->debug("FOUND " + tile->toString());
        return;
    }

    //Add adjacent vertices
    for (auto adjacentTile : tile->adjacents) {
        //Skip tile which is my back
        tile_index_t adjacentIndex = adjacentTile->index;
        if (adjacentIndex == vertex.back) {
            continue;
        }

        //Check if tile should be skipped, unless tile is the goal which is probably occupied by our entity
        if (adjacentIndex != goal->index) {
            bool tileInvalid = (tile->tileFlags & tileFlagsRequired) != tileFlagsRequired;
            bool entityOccupied = tile->entityFlags & entityFlagsMask;
            if (tileInvalid || entityOccupied) {
                continue;
            }
        }

        //Calculate G cost + accumulated previous cost
        path_cost_t g = vertex.g;
        g += adjacentTile->position.distanceSquared(tile->position);

        //Check if adjacent vertex should be updated if lower G than currently has
        //(because a shorter route has been found) or vertex is stale
        PathVertex& adjacentVertex = vertexes[adjacentIndex];
        if (g < adjacentVertex.g || staleVertex(adjacentVertex, adjacentTile)) {
            adjacentVertex.g = g;
            adjacentVertex.l = adjacentTile->entityFlags;
            adjacentVertex.back = vertexIndex;
        }

        //Check if we should add vertex to visit queue
        if (heuristic[adjacentIndex] == PATHFINDER_INFINITY) {
            calculateHeuristic(adjacentTile);
            queue.push(&adjacentVertex);
        }
    }
}

void AStar::calculateHeuristic(Tile* tile) {
    heuristic[tile->index] = tile->position.distanceSquared(goal->position);
}

PathFinderStatus AStar::getStatus() {
    return status;
}

Tile* AStar::getClosest() {
    return closest;
}

bool AStar::staleVertex(PathVertex& vertex, Tile* tile) {
    return vertex.g == PATHFINDER_INFINITY //Never visited
        || vertex.l != tile->entityFlags //Flags changed since last visit
        ;
}
