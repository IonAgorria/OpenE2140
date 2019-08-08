//
// Created by Ion Agorria on 22/06/19
//
#ifndef OPENE2140_ENTITY_CONFIG_H
#define OPENE2140_ENTITY_CONFIG_H

#include <unordered_map>
#include "engine/io/log.h"
#include "engine/core/common.h"
#include "engine/core/to_string.h"
#include "engine/io/has_config_data.h"

class IEntityFactory;
class AssetManager;
class Image;

/**
 * Contains per sprite set related data
 */
struct SpriteGroup {
    /** Name for this entry */
    std::string name = "";
    /** Images for this entry */
    std::vector<Image*> images;
    /** Total duration of all images */
    duration_t duration = 0;
    /** Should the animation loop? */
    bool loop = false;
};

/**
 * Base entity config containing the entity stats, type and such data
 */
class EntityConfig: public IHasConfigData, public entity_type_t, public IToString {
public:
    /*
     * Most common values
     */
    std::string code;
    std::string name;
    std::string type;

    /**
     * Sprites data
     */
    std::unordered_map<std::string, SpriteGroup> sprites;

    /**
     * Constructor
     */
    EntityConfig() = default;

    /**
     * Destructor
     */
    ~EntityConfig() override = default;

    /**
     * Loads data to entity config
     *
     * @param configData data store as entity config
     * @param factory which this entity config is being configured from
     */
    void loadData(const config_data_t& configData, const IEntityFactory* factory);

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EntityConfig)

    /**
     * Handles the loading and assemble of sprites in entity config as set of images
     *
     * @param factory which this entity config is being created from
     */
    void loadSprites(const IEntityFactory* factory);

    /*
     * IToString
     */

    std::string toString() const override;

    std::string toStringContent() const override;
};

#endif //OPENE2140_ENTITY_CONFIG_H
