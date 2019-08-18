//
// Created by Ion Agorria on 17/06/19
//
#ifndef OPENE2140_PATH_VERTEX_H
#define OPENE2140_PATH_VERTEX_H

#include "engine/core/types.h"
#include "engine/core/common.h"

using path_cost_t = uint16_t;

/**
 * Contains vertex data such as cost to goal and tile flags
 */
struct PathVertex {
    /** World tile index */
    tile_index_t index = 0;
    /** Previous tile index */
    tile_index_t back = 0;
    /** G Cost, distance from origin node, may be modified if shorter path is found to this node from origin */
    path_cost_t g = PATHFINDER_INFINITY;
    /** L value, last flags state from tile */
    tile_flags_t l = 0;
};

#endif //OPENE2140_PATH_VERTEX_H
