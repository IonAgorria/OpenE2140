//
// Created by Ion Agorria on 18/06/19
//
#ifndef OPENE2140_ASTAR_COMPARATOR_H
#define OPENE2140_ASTAR_COMPARATOR_H

class AStar;
class PathVertex;

/**
 * Custom priority queue comparator
 */
class AStarComparator {
public:
    /**
     * Pointer to astar which belongs this comparator
     */
    AStar* astar;

    /**
     * Compares the path vertexes
     *
     * @param v1
     * @param v2
     * @return
     */
    bool operator()(PathVertex& v1, PathVertex& v2);
};

#endif //OPENE2140_ASTAR_COMPARATOR_H
