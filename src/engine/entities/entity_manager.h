//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_ENTITY_MANAGER_H
#define OPENE2140_ENTITY_MANAGER_H

#include "engine/core/types.h"
#include "engine/core/macros.h"
#include "engine/io/log.h"
#include "engine/core/error_possible.h"

class AssetManager;
class IEntityFactory;
class Engine;
class Entity;
class EntityConfig;

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
    std::vector<std::unique_ptr<IEntityFactory>> factories;

public:
    /**
     * Constructs loader
     */
    explicit EntityManager(std::shared_ptr<Engine> engine);

    /**
     * Destructs loader
     */
    ~EntityManager() override;

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
     * Creates a new entity from provided entity config using the factories
     *
     * @param config to create
     * @return entity created, might point to null if none was build
     */
    std::shared_ptr<Entity> makeEntity(EntityConfig* config);

    /**
     * Creates a new entity from provided type using the factories
     *
     * @param type to create
     * @return entity created, might point to null if none was build
     */
    std::shared_ptr<Entity> makeEntity(const entity_type_t& type);

    /**
     * Creates a new entity from provided type using the factories
     *
     * @param kind of entity type
     * @param code of entity to create
     * @return entity created, might point to null if none was build
     */
    std::shared_ptr<Entity> makeEntity(entity_kind_t kind, const std::string& code);

    /**
     * Obtain the asset manager
     *
     * @return AssetManager
     */
    AssetManager* getAssetManager();
};

#endif //OPENE2140_ENTITY_MANAGER_H
