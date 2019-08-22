//
// Created by Ion Agorria on 22/8/19
//
#ifndef OPENE2140_ENTITY_STORE_H
#define OPENE2140_ENTITY_STORE_H

#include <vector>
#include <memory>
#include "engine/core/types.h"
#include "engine/core/macros.h"

class Entity;

/**
 * Stores the entities in different manners for both easy iterating and lookup
 */
class EntityStore {
protected:
    /**
     * Entities vector
     */
    std::vector<std::shared_ptr<Entity>> entities;

    /**
     * Lowest usable ID
     */
    entity_id_t lowestID = 0;

    /**
     * Last used entity id
     */
    entity_id_t lastEntityID = 0;

    /**
     * Does a internal lookup to find the lowest usable ID
     */
    void updateLowestID();

public:
    /**
     * Constructor
     */
    EntityStore() = default;

    /**
     * Destructor
     */
    ~EntityStore() = default;

    /**
     * Disable copy
     */
    NON_COPYABLE(EntityStore)

    /**
     * Clear the entity store
     */
    void clear();

    /**
     * Obtains a entity from store if any
     *
     * @param id to search
     * @return entity
     */
    std::shared_ptr<Entity> getEntity(entity_id_t id) const;

    /**
     * @return all entities in store
     */
    const std::vector<std::shared_ptr<Entity>>& getEntities() const;

    /**
     * Does insertion to entity store
     *
     * @param entity to add
     */
    entity_id_t add(std::shared_ptr<Entity> entity);

    /**
     * Does removal from entity store
     *
     * @param entity to remove
     */
    void remove(std::shared_ptr<Entity> entity);
};

#endif //OPENE2140_ENTITY_STORE_H
