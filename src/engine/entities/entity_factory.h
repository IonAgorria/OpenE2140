//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_ENTITY_FACTORY_H
#define OPENE2140_ENTITY_FACTORY_H

#include "engine/core/macros.h"
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
     * Log for object
     */
    log_ptr log;

    /**
     * Entity manager which this belongs to
     */
    EntityManager* manager = nullptr;

    /**
     * Loaded configs for entity types which this factory instances
     */
    std::vector<std::unique_ptr<EntityConfig>> configs;

    /**
     * Mapping for code to ids, this allows fast lookup of codes
     */
    std::unordered_map<std::string,entity_type_id_t> configCodes;

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
    ~IEntityFactory() override = default;

    /**
     * Disable copy
     */
    NON_COPYABLE(IEntityFactory)

    /**
     * Type name
     */
    TYPE_NAME_VIRTUAL();

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
     * Called when config is created
     *
     * @param config
     */
    virtual void setupEntityConfig(EntityConfig* config);

    /**
     * @return the file path containing the configs to use in this factory
     */
    virtual std::string getConfigPath() const = 0;

    /**
     * @return the default asset path which will be used unless is specified
     */
    virtual std::string getAssetPath() const;

    /**
     * @return the variants
     */
    virtual std::vector<std::string> getVariants() const;

    /**
     * @return creates the asset path using different components
     */
    virtual asset_path_t assembleAssetPath(const asset_path_t& path, const std::string& variant, const std::string& index) const;

    /**
     * @return creates the group name using different components
     */
    virtual std::string assembleGroupCode(const std::string& name, const std::string& variant, const std::string& collection) const;

    /**
     * @return the entity kind which this factory produces entities
     */
    virtual entity_kind_t getKind() const = 0;

    /**
     * Obtain config for entity id
     *
     * @param id of entity type
     * @return config if found
     */
    EntityConfig* getConfig(entity_type_id_t id);

    /**
     * Obtain config for entity code
     *
     * @param code of entity type
     * @return config if found
     */
    EntityConfig* getConfigCode(const std::string& code);

    /**
     * Instantiation of entity by the factory implementation
     *
     * @param config of entity to instantiate
     * @return entity
     */
    virtual std::shared_ptr<Entity> instanceEntity(EntityConfig* config) = 0;

    /**
     * Obtains log for this factory
     */
    log_ptr getLog() const;

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
