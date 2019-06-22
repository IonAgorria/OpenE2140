//
// Created by Ion Agorria on 13/06/19
//
#include <src/engine/simulation/world/tile.h>
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
    visitTile(vertex, nullptr);
}

void AStar::compute() {
    if (status != PathFinderStatus::Computing) {
        return;
    }
    //TODO
}

void AStar::visitTile(PathVertex& vertex, PathVertex* from) {
    if (from) {
        path_cost_t g = from->g; //TODO add method for calculating cost;
        if (g < vertex.g) {
            vertex.g = g;
            vertex.back = from->index;
        }
    } else {
        vertex.g = 0;
        vertex.back = vertex.index;
    }
}

PathFinderStatus AStar::getStatus() {
    return status;
}
