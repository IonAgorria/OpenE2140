//
// Created by Ion Agorria on 14/06/19
//
#include <src/engine/simulation/tile.h>
#include "path_request.h"

PathRequest::PathRequest() {
}

void PathRequest::initialize() {
    vertexes.clear();
    //TODO vertexes.resize(world size)
    //TODO Iterate and init each astar

}

PathVertex* PathRequest::getVertex(tile_index_t tile, PathVertex* from) {
    PathVertex* vertex = vertexes.at(tile).get();
    if (!vertex) {
        //Create a new vertex
        std::unique_ptr<PathVertex> uniqueVertex = std::make_unique<PathVertex>();
        vertex = uniqueVertex.get();
        vertexes[tile] = std::move(uniqueVertex);
        vertex->index = tile;
    }
    return vertex;
}