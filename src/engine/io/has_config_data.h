//
// Created by Ion Agorria on 24/07/19
//
#ifndef OPENE2140_HAS_CONFIG_DATA_H
#define OPENE2140_HAS_CONFIG_DATA_H

#include "engine/core/types.h"

/**
 * Implements code to handle config data containing objects
 */
class IHasConfigData {
protected:
    /**
     * Contains data to get config values from
     */
    config_data_t data;

public:
    /**
     * Destructor
     */
    virtual ~IHasConfigData() = default;

    /**
     * Sets the data from config data
     * @param content values to set
     */
    virtual void setData(const config_data_t& content) {
        data.update(content);
    }

    /**
     * Returns true if key exists, even if it's null
     *
     * @param key
     * @return
     */
    bool hasData(const config_key_t& key) const {
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
    T getData(const config_key_t& key, T defaultValue) const {
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
    T getData(const config_key_t& key, T& defaultValue, T& nullValue) const {
        const auto it = data.find(key);
        if (it != data.end()) {
            return it->is_null() ? nullValue : *it;
        }
        return defaultValue;
    }
};

#endif //OPENE2140_HAS_CONFIG_DATA_H
