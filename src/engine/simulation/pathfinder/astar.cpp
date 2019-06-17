//
// Created by Ion Agorria on 13/06/19
//
#include "engine/core/utils.h"
#include "astar.h"

AStar::AStar() {
}

void AStar::initialize() {
    queue.clear();
}

void AStar::plan(Tile* newStart, Tile* newGoal) {
    initialize();
    start = newStart;
    goal = newGoal;
    visitTile(goal, nullptr);
    status = PathFinderStatus::Computing;
}

void AStar::compute() {
}

void AStar::visitTile(Tile* tile, PathVertex* from) {
}

PathFinderStatus AStar::getStatus() {
    return status;
}
