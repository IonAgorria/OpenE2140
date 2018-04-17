//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_CONTAINER_H
#define OPENE2140_CONTAINER_H

#include <map>
#include "core/log.h"
#include "asset.h"

/**
 * A container stores different assets that can be accessed with path
 */
class Container {
protected:
    /**
     * Path of this container
     */
    std::string path;

    /**
     * Contains all assets in this container
     */
    std::unordered_map<std::string, std::unique_ptr<Asset>> assets;

public:
    /**
     * Container constructor
     */
    explicit Container(const std::string& path);

    /**
     * Container destructor
     */
    virtual ~Container();

    /**
     * Disable copy constructor
     */
    Container(const Container& other) = delete;

    /**
     * Disable copy operator
     */
    void operator=(const Container& other) = delete;

    /**
     * Load container contents
     *
     * @param log to use when loading
     */
    virtual bool load(const log_ptr& log) = 0;

    /**
     * @return the path of container
     */
    const std::string& getPath();

    /**
     * @return asset from container in specified path
     */
     const Asset& getAsset(const std::string& path);

};

#endif //OPENE2140_CONTAINER_H
