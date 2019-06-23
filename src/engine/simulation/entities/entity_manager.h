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
     * Asset processors
     */
    std::vector<std::unique_ptr<IEntityFactory>> factories;

public:
    /**
     * Constructs loader
     */
    EntityManager(std::shared_ptr<Engine> game);

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
};

#endif //OPENE2140_ENTITY_MANAGER_H
