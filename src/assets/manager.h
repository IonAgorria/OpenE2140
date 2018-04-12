//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_LOADER_H
#define OPENE2140_LOADER_H

#include <string>
#include <map>
#include "container.h"

/**
 * Handles the loading of different assets
 */
class Manager {
private:
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
     * Adds a container which contains assets (folder or mapped file with paths)
     *
     * @param path of container
     * @return if success
     */
    bool addContainer(std::string& path);

    /**
     * Loads an provided asset by path by searching in containers
     *
     * @param path to load
     * @return if success
     */
    bool loadAsset(std::string& path);

    /**
     * Gets the loaded asset
     *
     * @return asset
     */
    void getAsset(std::string& path);
};

#endif //OPENE2140_LOADER_H
