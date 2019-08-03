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
    config_data_t configData;

    /**
     * Keeps the dirty state (data was modified since last setData)
     */
    bool dirty;

public:
    /**
     * Destructor
     */
    virtual ~IHasConfigData() = default;

    /**
     * Loads the data from config data and resets dirty flag
     * @param content values to set
     */
    virtual void loadData(const config_data_t& content) {
        configData.update(content);
    }

    /**
     * Returns true if key exists, even if it's null
     *
     * @param key
     * @return
     */
    bool hasData(const config_key_t& key) const {
        const auto it = configData.find(key);
        return it != configData.end();
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
        const auto it = configData.find(key);
        if (it != configData.end() && !it->is_null()) {
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
        const auto it = configData.find(key);
        if (it != configData.end()) {
            return it->is_null() ? nullValue : *it;
        }
        return defaultValue;
    }

    /**
     * Sets the value in config data
     *
     * @tparam T type of value to obtain/return
     * @param key the key of value to access
     * @param value the value to set in key
     */
    template<typename T>
    void setData(const config_key_t& key, T& value) {
        configData[key] = value;
    }
};

#endif //OPENE2140_HAS_CONFIG_DATA_H
