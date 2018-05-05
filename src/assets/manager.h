//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_LOADER_H
#define OPENE2140_LOADER_H

#include <string>
#include <map>
#include "core/io/log.h"
#include "asset.h"

/**
 * Handles the loading of different assets
 */
class Manager {
private:
    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Contains all assets in this manager
     */
    std::unordered_map<asset_path, std::shared_ptr<Asset>> assets;

    /**
     * Number of assets loaded
     */
    int assetsCount;

    /**
     * Adds asset to manager in specified path
     *
     * @param asset to add
     * @return true if added or false if asset exists on same path
     */
    bool addAsset(std::shared_ptr<Asset> asset);

    /**
     * Each WD container file record struct
     */
    typedef struct {
        unsigned int fileOffset;
        unsigned int fileSize;
        unsigned int unused1;
        unsigned int unused2;
        unsigned int checkSum;
        unsigned int nameOffset;
    } FileRecord;
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
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Manager)

    /**
     * Gets the loaded asset
     *
     * @return asset
     */
    std::shared_ptr<Asset> getAsset(const asset_path& path);

    /**
     * @return the count of assets loaded
     */
    int getAssetsCount() {
        return assetsCount;
    }

    /**
     * Clears all loaded assets from manager
     */
    void clearAssets();

    /**
     * Loads containers required by this game
     *
     * @return true if success
     */
    bool loadContainers();

    /**
     * Loads a container which contains assets to get the contents (folder or mapped file with paths)
     *
     * @param path of containers root
     * @param name of container
     * @return type of container used or empty if none
     */
    bool loadContainer(const std::string& path, const std::string& name);

    /**
     * Load assets from WD file container and stores in manager
     *
     * @param path to use as assets root
     * @param name of container
     * @return amount of loaded assets or -1 if error occurred
     */
    int loadContainerWD(const std::string& path, const std::string& name);

    /**
     * Load assets from directory container and stores in manager
     *
     * @param path to use as assets root
     * @param name of container
     * @return amount of loaded assets or -1 if error occurred
     */
    int loadContainerDir(const std::string& path, const std::string& name);
};

#endif //OPENE2140_LOADER_H
