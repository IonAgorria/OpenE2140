//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PRIORITY_QUEUE_H
#define OPENE2140_PRIORITY_QUEUE_H

#include <queue>

/**
 * Priority queue with some extras
 * Uses greater comparator by default to order lower elements first
 */
template<typename T, typename Comparator = std::greater<T>>
class PriorityQueue: public std::priority_queue<T, std::deque<T>, Comparator> {
public:
    Comparator& getComparator() {
        return this->comp;
    }

    void clear() {
        this->c.clear();
    }
};

#endif //OPENE2140_PRIORITY_QUEUE_H
