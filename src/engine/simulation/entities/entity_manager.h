//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_ENTITY_MANAGER_H
#define OPENE2140_ENTITY_MANAGER_H

#include "engine/core/common.h"
#include "engine/io/log.h"
#include "engine/core/error_possible.h"

class IEntityFactory;
class Engine;

/**
 * Handles the entity config loading, storage and entity instantiation using config
 */
class EntityManager: public IErrorPossible {
private:
    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Engine pointer
     */
    std::shared_ptr<Engine> engine;

    /**
     * Entity factories for each kind
     */
    std::unordered_map<entity_kind_t, std::unique_ptr<IEntityFactory>> factories;

public:
    /**
     * Constructs loader
     */
    EntityManager(std::shared_ptr<Engine> entity);

    /**
     * Destructs loader and cleans any loaded assets
     */
    ~EntityManager();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EntityManager)

    /**
     * Registers entity factory
     *
     * @param factory to register
     */
    void addEntityFactory(std::unique_ptr<IEntityFactory> factory);

    /**
     * Clears loaded configs
     */
    void clear();

    /**
     * Loads config for entities
     */
    void load();

    /**
     * Creates a new entity from provided type using the factories
     *
     * @param type to create
     * @return entity created, might point to null if none was build
     */
    std::shared_ptr<Entity> makeEntity(entity_type_t type);
};

#endif //OPENE2140_ENTITY_MANAGER_H
