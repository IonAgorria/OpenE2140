//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_ASSETMANAGER_H
#define OPENE2140_ASSETMANAGER_H

#include <string>
#include <map>
#include <list>
#include "asset.h"
#include "core/errorpossible.h"
#include "math/vector2.h"
#include "io/log.h"

/**
 * Handles the loading of different assets
 */
class Game;
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
    std::shared_ptr<Game> game;

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
    AssetManager(std::shared_ptr<Game> game);

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
     * @throw  std::out_of_range  If no such asset exists
     */
    Asset* getAsset(const asset_path& path);

    /**
     * Gets the loaded asset with specified cast
     *
     * @return asset
     * @throw  std::out_of_range  If no such asset exists
     */
    template <typename T>
    T* getAsset(const asset_path& path);

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
     * Clears all loaded assets from manager
     */
    void clearAssets();

    /**
     * Loads the assets data from files into memory
     *
     * @param assetsRoot directory to use as assets root
     * @param containerNames names of containers to scan in assets root
     */
    void loadAssets(const std::string& assetsRoot, const std::vector<std::string> containerNames);

    /**
     * (Re)Loads the volatile assets data using memory data
     */
    void refreshAssets();
};

#endif //OPENE2140_ASSETMANAGER_H
