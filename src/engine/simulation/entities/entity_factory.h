//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_ENTITY_FACTORY_H
#define OPENE2140_ENTITY_FACTORY_H

#include "engine/core/error_possible.h"

class EntityManager;
/**
 * Processor for asset manager loading process
 */
class IEntityFactory: public IErrorPossible {
protected:
    /**
     * Entity manager which this belongs to
     */
    EntityManager* manager;

public:
    /**
     * Destructor
     */
    virtual ~IEntityFactory() = default;

    /**
     * Assigns the current manager
     */
    void setManager(EntityManager* current) {
        manager = current;
    }
};

#endif //OPENE2140_ENTITY_FACTORY_H
