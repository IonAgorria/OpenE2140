//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_ENTITY_FACTORY_H
#define OPENE2140_ENTITY_FACTORY_H

#include "engine/core/common.h"
#include "engine/core/error_possible.h"

class EntityManager;
class Entity;

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

    /**
     * Called when manager clears
     */
    virtual void clear() {};

    /**
     * Called when manager loads
     */
    virtual void load() {};

    /**
     * @return the entity kind which this factory produces entities
     */
    virtual entity_kind_t getKind() = 0;

    /**
     * Handles the entity request
     *
     * @param id type of entity in this factory kind domain
     * @return entity created
     */
     virtual std::shared_ptr<Entity> makeEntity(entity_type_id_t id) = 0;
};

#endif //OPENE2140_ENTITY_FACTORY_H
