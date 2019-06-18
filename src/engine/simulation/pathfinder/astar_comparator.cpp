//
// Created by Ion Agorria on 18/06/19
//

#include "path_vertex.h"
#include "astar.h"
#include "astar_comparator.h"

bool AStarComparator::operator()(PathVertex& v1, PathVertex& v2) {
    path_cost_t f1 = v1.g + astar->heuristic[v1.index];
    path_cost_t f2 = v2.g + astar->heuristic[v2.index];
    return f1 > f2;
}