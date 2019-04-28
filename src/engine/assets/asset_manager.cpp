//
// Created by Ion Agorria on 8/04/18
//
#include "core/common.h"
#include "core/utils.h"
#include "io/log.h"
#include "asset_palette.h"
#include "asset_image.h"
#include "graphics/renderer.h"
#include "src/engine/core/engine.h"
#include "asset_manager.h"
#include "asset_processor.h"

//Rect pack
#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_LARGE_RECTS
#include "stb_rect_pack.h"

AssetManager::AssetManager(std::shared_ptr<Engine> game): game(game) {
    log = Log::get("Assets");
}

AssetManager::~AssetManager() {
    log->debug("Closing");
    clearAssets();
    processors.clear();
}

void AssetManager::addAssetProcessor(std::unique_ptr<IAssetProcessor> processor) {
    processor->setManager(this);
    processors.push_back(std::move(processor));
}

const std::unordered_map<asset_path, std::unique_ptr<Asset>>& AssetManager::getAssets() {
    return assets;
}

bool AssetManager::addAsset(std::unique_ptr<Asset> asset) {
    if (!asset) {
        error = "Asset to add is null";
        return false;
    }
    const std::string& path = asset->getPath();
    if (assets[path]) {
        error = "Asset already present: '" + path + "'";
        return false;
    }
    assets[path] = std::move(asset);
    assetsCount++;
    return true;
}

bool AssetManager::removeAsset(const asset_path& path) {
    if (assets.erase(path) == 0) {
        error = "Asset is not present: '" + path + "'";
        return false;
    }
    assetsCount--;
    return true;
}

Asset* AssetManager::getAsset(const asset_path& path) {
    return Utils::getPointerFromUnorderedMap(assets, path);
}

template <typename T>
T* AssetManager::getAsset(const asset_path& path) {
    return dynamic_cast<T*>(getAsset(path));
}

std::shared_ptr<Image> AssetManager::getImage(const asset_path& path) {
    std::shared_ptr<Image> image;
    AssetImage* assetImage = getAsset<AssetImage>(path);
    if (assetImage) {
        image = assetImage->getImage();
    }
    return image;
}

int AssetManager::getAssetsCount() {
    return assetsCount;
}

void AssetManager::clearAssets() {
    //Delete all stored assets
    if (assetsCount) {
        log->debug("Deleting assets");
    }
    assets.clear();
    assetsCount = 0;
}

void AssetManager::loadAssets(const std::string& assetsRoot, const std::vector<std::string> containerNames) {
    clearAssets();
    log->debug("Loading assets");

    //Scan assets from containers by checking different paths that might contain assets
    std::list<std::string> assetDirPaths = {
            assetsRoot,                                                 //Current directory
            Utils::getInstallPath() + assetsRoot,                       //Installation directory
            Utils::getParentPath(Utils::getInstallPath()) + assetsRoot, //Parent of installation directory
    };
    for (std::string name : containerNames) {
        log->debug("Loading '{0}'", name);
        bool found = false;
        for (std::string path : assetDirPaths) {
            for (std::unique_ptr<IAssetProcessor>& processor : processors) {
                found |= processor->scanContainer(path, name);
                error = processor->getError();
            }
            if (found || !error.empty()) break;
        }

        //Nothing was found
        if (!found) {
            std::string text = "Error loading game data for directory/file '" + name + "'\n";
            text += "Check if game files are correctly set and are accessible inside following paths: \n";
            for (std::string path : assetDirPaths) {
                text += path + "\n";
            }
            //Add prev error
            if (!error.empty()) {
                text += error;
            }
            error = text;
            return;
        }
    }

    //Process intermediate assets to extract assets inside assets
    for (std::unique_ptr<IAssetProcessor>& processor : processors) {
        processor->processIntermediates();
        error = processor->getError();
    }
    if (!error.empty()) return;

    //Refresh the assets
    refreshAssets();
    if (!error.empty()) return;

    //Print loaded assets
    //for (std::pair<asset_path, std::shared_ptr<Asset>> pair : assets) log->debug(pair.first);

    log->debug("Loaded {0} assets", assetsCount);
}

void AssetManager::refreshAssets() {
    Renderer* renderer = game->getRenderer();
    if (!renderer) {
        error = "Renderer is not available";
        return;
    }
    unsigned int textureSize = renderer->getMaxTextureSize();
    unsigned int batchSize = (textureSize * textureSize) / (64 * 64);

    //Iterate all assets and handle by asset type
    std::vector<AssetImage*> assetImages;
    std::vector<AssetImage*> assetImagesWithPalettes;
    for (std::unordered_map<asset_path, std::unique_ptr<Asset>>::iterator pair = assets.begin(); pair != assets.end(); ++pair) {
        //Handle image assets
        AssetImage* assetImage = dynamic_cast<AssetImage*>(pair->second.get());
        if (assetImage) {
            assetImage->assignImage(nullptr);
            std::shared_ptr<AssetPalette> assetPalette = assetImage->getAssetPalette();
            if (assetPalette) {
                assetPalette->assignPalette(nullptr);
                assetImagesWithPalettes.push_back(assetImage);
            } else {
                assetImages.push_back(assetImage);
            }
            continue;
        }
    }

    //Process the images without palettes
    processImages(textureSize, batchSize, assetImages, false);
    if (!error.empty()) return;

    //Palettes must be refreshed before images
    for (AssetImage* assetImage : assetImagesWithPalettes) {
        //Create a new palette
        std::shared_ptr<Palette> palette = std::make_shared<Palette>(ASSET_PALETTE_COUNT, false);
        error = palette->getError();
        if (!error.empty()) return;

        //Set or replace with the new palette
        std::shared_ptr<AssetPalette> assetPalette = assetImage->getAssetPalette();
        if (!assetPalette->assignPalette(palette)) {
            error = assetPalette->getError();
            return;
        }
    }

    //Process the images with palettes
    processImages(textureSize, batchSize, assetImagesWithPalettes, true);
    if (!error.empty()) return;
}

void AssetManager::processImages(
        const unsigned int textureSize, unsigned int batchSize,
        std::vector<AssetImage*>& assetImages, bool withPalette
    ) {
    //Init structures
    stbrp_context context;
    std::vector<stbrp_node> nodes(textureSize * 2);
    std::vector<stbrp_rect> rects(batchSize);

    int retryCount = 0;
    size_t totalCount = assetImages.size();
    size_t lastSize = 0;
    size_t atlasIndex = 0;
    //This is used to detect stalled packing (no advances in successive attempts)
    while (!assetImages.empty() && lastSize != assetImages.size()) {
        lastSize = assetImages.size();

        //Create the base image
        std::shared_ptr<Image> atlasImage = std::make_shared<Image>(Vector2(textureSize), withPalette);
        error = atlasImage->getError();
        if (!error.empty()) return;

        //Setup the rects and add the index so we know which image does reference
        unsigned int imageCount = (unsigned int) std::min((size_t) batchSize, lastSize);
        for (unsigned int i = 0; i < imageCount; ++i) {
            unsigned int index = imageCount - 1 - i;
            stbrp_rect& rect = rects.at(index);
            AssetImage* assetImage = assetImages.at(index);
            Vector2 imageSize = assetImage->getImageSize();
            if (0 > imageSize.x
             || 0 > imageSize.y
             || textureSize < (unsigned) imageSize.x
             || textureSize < (unsigned) imageSize.y) {
                error = "This asset image exceeds the maximum texture size allowed" + assetImage->getPath();
                return;
            }
            rect.id = index;
            rect.w = imageSize.x;
            rect.h = imageSize.y;
            rect.x = 0;
            rect.y = 0;
            rect.was_packed = 0;
        }

        //Prepare the rect packer and do it
        stbrp_init_target(
                &context,
                textureSize,
                textureSize,
                nodes.data(),
                textureSize
        );
        stbrp_pack_rects(&context, rects.data(), imageCount);

        //Get the rects that were packed and use them for actual image setup
        for (unsigned int i = 0; i < imageCount; ++i) {
            unsigned int index = imageCount - 1 - i;
            //Get rectangle
            stbrp_rect& rect = rects.at(index);
            if ((unsigned) rect.id != index) {
                //This shouldn't happen unless rects become reordered
                error = "Rect id doesn't match index";
                return;
            }
            if (rect.was_packed == 0) {
                //Okay, to next round
                //log->debug("Retry {0}", index);
                retryCount++;
                continue;
            }
            Rectangle rectangle(rect.x, rect.y, rect.w, rect.h);

            //Fetch and remove asset image from queue
            std::vector<AssetImage*>::iterator it = assetImages.begin() + index;
            AssetImage* assetImage = *it;
            assetImages.erase(it);

            //Create a subset image which inherits the atlas image using the rectangle
            std::shared_ptr<Image> subImage = std::make_shared<Image>(rectangle, withPalette, atlasImage);
            error = subImage->getError();
            if (!error.empty()) return;

            //Assign palette
            if (withPalette) {
                std::shared_ptr<AssetPalette> assetPalette = assetImage->getAssetPalette();
                subImage->setPalette(assetPalette->getPalette());
            }

            //Assign the subset image to the asset, this loads the asset data to image
            //log->debug("{0}:{1} {2} {3}", atlasIndex, i, rectangle.toString(), assetImage->getPath());
            bool result = assetImage->assignImage(subImage);
            if (!result) {
                error = assetImage->getError();
                if (error.empty()) error = "Error assigning image to asset";
                error += "\nAsset: " + assetImage->getPath();
                return;
            }
        }

        log->debug("Atlas {0} contains {1} images", atlasIndex, lastSize - assetImages.size());
        atlasIndex++;
    }

    //Done
    log->debug("Packing atlas count {0} image count {1} retry count {2}", atlasIndex, totalCount, retryCount);
    if (!assetImages.empty()) {
        error = "Packing failed for " + std::to_string(assetImages.size()) + " assets";
        return;
    }
}
