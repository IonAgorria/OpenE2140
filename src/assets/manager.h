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
    std::map<std::string, Container*> containers;
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
    void operator=(Manager const &other) = delete;

    /**
     * Loads a container which contains assets to get the contents (folder or mapped file with paths)
     *
     * @param path of container
     * @return true if success
     */
    bool loadContainer(const std::string& path);

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
    Asset getAsset(const std::string& path);
};

#endif //OPENE2140_LOADER_H
