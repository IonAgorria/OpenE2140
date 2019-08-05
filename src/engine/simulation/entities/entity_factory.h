//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_ENTITY_FACTORY_H
#define OPENE2140_ENTITY_FACTORY_H

#include "engine/core/common.h"
#include "engine/core/error_possible.h"
#include "entity_config.h"

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

    /**
     * Loaded configs for entity types which this factory instances
     */
    std::vector<std::unique_ptr<EntityConfig>> configs;

    /**
     * Instantiation of entity by the factory implementation
     *
     * @param id
     * @return
     */
    virtual std::shared_ptr<Entity> instanceEntity(entity_type_id_t id, EntityConfig* config) = 0;

    /**
     * Attempts to load a config
     */
    void loadConfig(const std::string& path);

public:
    /**
     * Constructor
     */
    IEntityFactory() = default;

    /**
     * Destructor
     */
    virtual ~IEntityFactory() = default;

    /**
     * Disable copy
     */
    NON_COPYABLE(IEntityFactory)

    /**
     * Assigns the current manager
     */
    void setManager(EntityManager* current);

    /**
     * Called when manager clears
     */
    virtual void clear();

    /**
     * Called when manager loads
     */
    virtual void load();

    /**
     * Called when loading a entity config
     */
    virtual void loadEntityConfig(config_data_t& data);

    /**
     * @return the file path containing the configs to use in this factory
     */
    virtual const std::string getConfigPath() = 0;

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
    std::shared_ptr<Entity> makeEntity(entity_type_id_t id);

    /*
     * AssetManager proxy
     */

    /**
     * Gets the loaded image from an asset
     *
     * @return image
     */
    Image* getImage(const asset_path_t& path) const;
};

#endif //OPENE2140_ENTITY_FACTORY_H
