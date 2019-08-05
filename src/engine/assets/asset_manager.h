//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_ASSETMANAGER_H
#define OPENE2140_ASSETMANAGER_H

#include <map>
#include <list>
#include "engine/core/error_possible.h"
#include "engine/core/utils.h"
#include "engine/math/vector2.h"
#include "engine/io/log.h"

/**
 * Handles the loading of different assets
 */
class Engine;
class Asset;
class AssetImage;
class Image;
class IAssetProcessor;
class AssetManager: public IErrorPossible {
private:
    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Game which assets belong to
     */
    std::shared_ptr<Engine> engine;

    /**
     * Asset processors
     */
    std::vector<std::unique_ptr<IAssetProcessor>> processors;

    /**
     * Asset containers
     */
    std::unordered_map<std::string, bool> assetContainers;

    /**
     * Contains all assets in this manager
     */
    std::unordered_map<asset_path_t, std::unique_ptr<Asset>> assets;

    /**
     * Number of assets loaded
     */
    int assetsCount;

    /**
     * Loads the assets data in the container from files into memory
     *
     * @param assetRoots paths to search the container
     * @param containerName name of container to load
     * @param required generates error if missing
     */
    void loadAssetContainer(const std::vector<std::string>& assetRoots, const std::string& containerName, bool required);

    /**
     * Processes the images
     */
    void processImages(unsigned int textureSize, unsigned int batchSize, std::vector<AssetImage*>& assetImages, bool withPalette);
public:
    /**
     * Constructs loader
     */
    AssetManager(std::shared_ptr<Engine> engine);

    /**
     * Destructs loader and cleans any loaded assets
     */
    ~AssetManager();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(AssetManager)

    /**
     * Registers asset processor
     *
     * @param processor to register
     */
    void addAssetProcessor(std::unique_ptr<IAssetProcessor> processor);

    /**
     * Gets the loaded assets map
     *
     * @return assets
     */
    const std::unordered_map<asset_path_t, std::unique_ptr<Asset>>& getAssets() const;

    /**
     * Adds asset to manager in specified path
     *
     * @param asset to add
     * @return true if added or false if asset exists on same path
     */
    bool addAsset(std::unique_ptr<Asset> asset);

    /**
     * Removes asset from manager in specified path
     *
     * @param asset to remove
     * @return true if removed
     */
    bool removeAsset(const asset_path_t& path);

    /**
     * Gets the loaded asset with specified cast
     *
     * @return asset
     */
    template <typename T = Asset>
    T* getAsset(const asset_path_t& path) const {
        auto it = assets.find(path);
        if (it == assets.end()) {
            //Not found
            return nullptr;
        }
        //Get asset and cast if need
        Asset* asset = (*it).second.get();
        return dynamic_cast<T*>(asset);
    }

    /**
     * Gets the loaded image from an asset
     *
     * @return image
     */
    std::shared_ptr<Image> getImage(const asset_path_t& path) const;

    /**
     * @return the count of assets loaded
     */
    int getAssetsCount();

    /**
     * Clears all loaded assets from manager
     */
    void clearAssets();

    /**
     * Registers asset container name
     *
     * @param containerName name of container to load
     * @param required if required then engine generates an error indicating the absence of container
     */
    void registerAssetContainer(const std::string& containerName, bool required);

    /**
     * Loads the assets data from files into memory
     */
    void loadAssets();

    /**
     * Processes intermediate assets
     */
    void processIntermediates();

    /**
     * (Re)Loads the volatile assets data using memory data
     */
    void refreshAssets();
};

#endif //OPENE2140_ASSETMANAGER_H
