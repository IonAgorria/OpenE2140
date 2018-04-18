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
private:
    /**
     * Contains all assets in this container
     */
    std::unordered_map<std::string, std::shared_ptr<Asset>> assets;

protected:
    /**
     * Path of containers root
     */
    const std::string path;

    /**
     * Name of this container
     */
    const std::string name;

    /**
     * Adds asset to container in specified path
     */
    void addAsset(const std::string& path, std::shared_ptr<Asset> asset);
public:
    /**
     * Container constructor
     *
     * @param path folder for containers
     * @param name of this container
     */
    explicit Container(const std::string& path, const std::string& name);

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
    virtual bool load(const log_ptr log) = 0;

    /**
     * @return the path of container
     */
    const std::string& getPath();

    /**
     * @return the name of container
     */
    const std::string& getName();

    /**
     * @return asset from container in specified path
     */
    std::shared_ptr<Asset> getAsset(const std::string& path);

};

#endif //OPENE2140_CONTAINER_H
