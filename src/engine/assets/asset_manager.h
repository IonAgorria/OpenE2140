//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_ASSETMANAGER_H
#define OPENE2140_ASSETMANAGER_H

#include <map>
#include <list>
#include "core/error_possible.h"
#include "core/utils.h"
#include "math/vector2.h"
#include "io/log.h"

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
    std::shared_ptr<Engine> game;

    /**
     * Asset processors
     */
    std::vector<std::unique_ptr<IAssetProcessor>> processors;

    /**
     * Contains all assets in this manager
     */
    std::unordered_map<asset_path, std::unique_ptr<Asset>> assets;

    /**
     * Number of assets loaded
     */
    int assetsCount;

    /**
     * Processes the images
     */
    void processImages(unsigned int textureSize, unsigned int batchSize, std::vector<AssetImage*>& assetImages, bool withPalette);
public:
    /**
     * Constructs loader
     */
    AssetManager(std::shared_ptr<Engine> game);

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
    const std::unordered_map<asset_path, std::unique_ptr<Asset>>& getAssets() const;

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
    bool removeAsset(const asset_path& path);

    /**
     * Gets the loaded asset
     *
     * @return asset
     */
    Asset* getAsset(const asset_path& path);

    /**
     * Gets the loaded asset with specified cast
     *
     * @return asset
     */
    template <typename T>
    T* getAsset(const asset_path& path) {
        Asset* asset = Utils::getPointerFromUnorderedMap(assets, path);
        return dynamic_cast<T*>(asset);
    }

    /**
     * Gets the loaded image from an asset
     *
     * @return image
     */
    std::shared_ptr<Image> getImage(const asset_path& path);

    /**
     * @return the count of assets loaded
     */
    int getAssetsCount();

    /**
     * Sets the asset roots available
     *
     * @param roots
     */
    static void getAssetRoots(std::vector<std::string>& roots);

    /**
     * Clears all loaded assets from manager
     */
    void clearAssets();

    /**
     * Loads the assets data from files into memory
     *
     * @param assetsRoot directory to use as assets root
     * @param containerNames names of container to scan in assets root
     * @param required flag to throw error if container is not found
     */
    void loadAssets(const std::vector<std::string>& assetRoots, const std::string& containerName, bool required);

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
