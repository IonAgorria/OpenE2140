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
     * Entity map to vector
     */
    std::unordered_map<entity_id_t, std::shared_ptr<Entity>> entitiesMap;

    /**
     * Entity map to vector
     */
    std::unordered_map<entity_type_key_t, std::vector<std::shared_ptr<Entity>>> entitiesTypeMap;

    /**
     * Last used entity id
     */
    entity_id_t lastEntityID = 0;

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
     * @return all entities in store that have the same type
     */
    const std::vector<std::shared_ptr<Entity>>& getEntitiesByType(const entity_type_t& type);

    /**
     * Does insertion to entity store
     *
     * @param entity to add
     */
    entity_id_t add(const std::shared_ptr<Entity>& entity);

    /**
     * Does removal from entity store
     *
     * @param entity to remove
     */
    void remove(const std::shared_ptr<Entity>& entity);

    /**
     * Transforms entity type key to entity type
     */
    static entity_type_t entityTypeFromKey(entity_type_key_t key);

    /**
     * Transforms entity type to entity type key
     */
    static entity_type_key_t entityKeyFromType(const entity_type_t& type);
};

#endif //OPENE2140_ENTITY_STORE_H
