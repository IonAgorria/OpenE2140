//
// Created by Ion Agorria on 19/05/19
//
#ifndef OPENE2140_MOVABLE_H
#define OPENE2140_MOVABLE_H

#include "entity.h"

/**
 * Movable entity with velocity
 */
class Movable: public Entity {
protected:
    /**
     * Entity velocity
     */
    Vector2 velocity;

public:
    //TYPE_NAME(Movable);
    const std::string type_name = std::string("Movable");
};

#endif //OPENE2140_MOVABLE_H
