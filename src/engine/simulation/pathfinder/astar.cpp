//
// Created by Ion Agorria on 13/06/19
//
#include "engine/simulation/world/world.h"
#include "astar.h"
#include "path_request.h"

AStar::AStar(PathRequest* request): request(request) {
    queue.getComparator().astar = this;
}

void AStar::initialize() {
    queue.clear();
    heuristic.clear();
    heuristic.resize(request->getVertexes().size(), PATHFINDER_INFINITY);
}

void AStar::plan(Tile* newStart, Tile* newGoal) {
    if (status == PathFinderStatus::Computing
     && start == newStart && goal == newGoal) {
        //No need to initialize
        return;
    }
    initialize();
    start = newStart;
    goal = newGoal;
    status = PathFinderStatus::Computing;
    //Create initial vertex
    PathVertex* vertex = &request->getVertexes()[start->index];
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
    while (!queue.empty()) {
        //Get the top vertex to scan next
        PathVertex vertex = *queue.top();
        queue.pop();

        //Visit it
        PathVertex* vertexFrom = vertex.back == vertex.index ? nullptr : &vertexes[vertex.back];
        visitTile(world, vertexes, vertex, vertexFrom);
    }
}

void AStar::visitTile(World* world, std::vector<PathVertex>& vertexes, PathVertex& vertex, PathVertex* vertexFrom) {
    Tile* tile = world->getTile(vertex.index);
    bool updated = false;
    if (vertexFrom) {
        Tile* tileFrom = world->getTile(vertexFrom->index);

        //Calculate G cost + accumulated previous cost
        path_cost_t g = vertexFrom->g;
        g += tileFrom->position.distanceSquared(tile->position);

        //If our is lower G then set from
        if (g < vertex.g) {
            updated = true;
            vertex.g = g;
            vertex.back = vertexFrom->index;
        }
        //Tile flags changed, set to update
        if (tile->tileFlags != vertex.l) {
            updated = true;
        }
    } else {
        updated = true;
        vertex.g = 0;
        vertex.back = vertex.index;
    }


    //Check if it's the goal
    if (vertex.index == goal->index) {
        vertex.l = tile->tileFlags;
        status = PathFinderStatus::Success;
        queue.clear();
        return;
    }

    //Update flags and add adjancent vertices
    if (updated) {
        vertex.l = tile->tileFlags;
        for (auto adjacentTile : tile->adjacents) {
            PathVertex& adjacent = vertexes[adjacentTile->index];
            if (adjacentTile->tileFlags != adjacent.l
            || adjacent.g == PATHFINDER_INFINITY) {
                queue.push(&adjacent);
            }
        }
    }
}

PathFinderStatus AStar::getStatus() {
    return status;
}
