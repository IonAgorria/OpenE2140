//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_LOADER_H
#define OPENE2140_LOADER_H

#include <string>
#include <map>
#include "core/log.h"
#include "container.h"

/**
 * Handles the loading of different assets
 */
class Manager {
private:
    /** Log for object */
    log_ptr log;
    /** Containers in this manager */
    std::map<std::string, std::unique_ptr<Container>> containers;
public:
    /**
     * Constructs loader
     */
    Manager();

    /**
     * Destructs loader and cleans any loaded assets
     */
    ~Manager();

    /**
     * Disable copy constructor
     */
    Manager(const Manager& other) = delete;

    /**
     * Disable copy operator
     */
    void operator=(const Manager& other) = delete;

    /**
     * Loads a container which contains assets to get the contents (folder or mapped file with paths)
     *
     * @param path of containers root
     * @param name of container
     * @return type of container used or empty if none
     */
    const std::string loadContainer(const std::string& path, const std::string& name);

    /**
     * Loads containers required by this game
     *
     * @return true if success
     */
    bool loadContainers();

    /**
     * Loads an provided asset by path by searching in containers
     *
     * @param path to load
     * @return true if success
     */
    bool loadAsset(const std::string& path);

    /**
     * Gets the loaded asset
     *
     * @return asset
     */
    std::shared_ptr<Asset> getAsset(const std::string& path);
};

#endif //OPENE2140_LOADER_H
