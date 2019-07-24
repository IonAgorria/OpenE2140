//
// Created by Ion Agorria on 22/06/19
//
#ifndef OPENE2140_ENTITY_CONFIG_H
#define OPENE2140_ENTITY_CONFIG_H

#include <unordered_map>
#include <src/engine/io/log.h>
#include "engine/core/types.h"
#include "engine/io/has_config_data.h"

class Image;

/**
 * Contains structured data for sprite group
 */
struct SpriteGroup {
    /**
     * Name of this sprite group
     */
    std::string name;

    /**
     * Type of this sprite group
     */
    uint16_t type;

    /**
     * Images in this group
     */
    std::vector<Image*> images;
};

/**
 * Base entity config containing the entity stats, type and such data
 */
class EntityConfig: public entity_type_t, public IHasConfigData {
public:
    /*
     * Most common values
     */
    std::string name;
    std::string type;

    /**
     * Sprites data
     */
    std::unordered_map<std::string, SpriteGroup> sprites;

    /**
     * Constructor
     */
    EntityConfig(config_data_t& configData) {
        setData(configData);
    }

    /**
     * Destructor
     */
    virtual ~EntityConfig() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EntityConfig)

    void setData(config_data_t& content) {
        IHasConfigData::setData(content);
        name = getData<const std::string>("name", "");
        type = getData<const std::string>("type", "");
        config_data_t spritesData = data["sprites"];
        if (spritesData.is_object()) {
            for (config_data_t::iterator entry = spritesData.begin(); entry != spritesData.end(); ++entry) {
                SpriteGroup group;
                group.name = entry.key();
                //TODO
                sprites[group.name] = std::move(group);
            }
        }
    }
};

#endif //OPENE2140_ENTITY_CONFIG_H
