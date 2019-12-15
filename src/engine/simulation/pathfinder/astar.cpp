//
// Created by Ion Agorria on 13/06/19
//
#include "engine/simulation/world/world.h"
#include "engine/simulation/world/tile.h"
#include "engine/core/utils.h"
#include "astar.h"
#include "path_request.h"

AStar::AStar(PathRequest* request): request(request) {
    queue.getComparator().astar = this;
}

void AStar::initialize() {
    queue.clear();
    heuristic.clear();
    heuristic.resize(request->getVertexes().size(), 0);
}

void AStar::plan(Tile* newStart, Tile* newGoal) {
    initialize();
    start = newStart;
    goal = newGoal;
    status = PathFinderStatus::Computing;
    //Create initial vertex
    PathVertex& vertex = request->getVertexes()[goal->index];
    vertex.l = goal->tileFlags;
    visitTile(request->getWorld(), vertex, nullptr);
}

void AStar::compute() {
    World* world = request->getWorld();
    if (status != PathFinderStatus::Computing || !world) {
        return;
    }
    while (!queue.empty()) {
        PathVertex vertex = *queue.top();
    }
}

void AStar::visitTile(World* world, PathVertex& vertex, PathVertex* vertexFrom) {
    Tile* tile = world->getTile(vertex.index);
    if (vertexFrom) {
        Tile* tileFrom = world->getTile(vertexFrom->index);
        path_cost_t g = vertexFrom->g;
        g += tileFrom->position-
        //TODO add method for calculating cost;
        if (g < vertex.g) {
            vertex.g = g;
            vertex.back = vertexFrom->index;
        }
    } else {
        vertex.g = 0;
        vertex.back = vertex.index;
    }
    for (auto adj : vertex) {

    }
}

PathFinderStatus AStar::getStatus() {
    return status;
}
