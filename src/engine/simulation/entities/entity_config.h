//
// Created by Ion Agorria on 22/06/19
//
#ifndef OPENE2140_ENTITY_CONFIG_H
#define OPENE2140_ENTITY_CONFIG_H

#include <unordered_map>
#include "engine/core/types.h"
#include "engine/io/config.h"

/**
 * Contains structured data for sprite group
 */
struct SpriteGroup {
};

/**
 * Base entity config containing the entity stats, type and such data
 */
class EntityConfig: public entity_type_t {
protected:
    /**
     * Contains data to get config values from
     */
    config_data_t data;

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
        data.update(configData);
        name = get<const std::string>("name", "");
        type = get<const std::string>("type", "");
    }

    /**
     * Destructor
     */
    virtual ~EntityConfig() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EntityConfig)

    /**
     * Returns true if key exists, even if it's null
     *
     * @param key
     * @return
     */
    bool has(const config_key_t& key) const {
        const auto it = data.find(key);
        return it != data.end();
    }

    /**
     * Return default value if key is not present or has null value
     *
     * @tparam T type of value to obtain/return
     * @param key the key of value to access
     * @param defaultValue value to return if key is not found/null
     * @return value
     */
    template<typename T>
    T get(const config_key_t& key, T defaultValue) const {
        const auto it = data.find(key);
        if (it != data.end() && !it->is_null()) {
            return *it;
        }
        return defaultValue;
    }

    /**
     * Return default value if key is not present or null value if has null value
     *
     * @tparam T type of value to obtain/return
     * @param key the key of value to access
     * @param defaultValue value to return if key is not found
     * @param nullValue value to return if key is null
     * @return value
     */
    template<typename T>
    T get(const config_key_t& key, T& defaultValue, T& nullValue) const {
        const auto it = data.find(key);
        if (it != data.end()) {
            return it->is_null() ? nullValue : *it;
        }
        return defaultValue;
    }
};

#endif //OPENE2140_ENTITY_CONFIG_H
