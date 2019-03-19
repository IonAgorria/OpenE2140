//
// Created by Ion Agorria on 8/04/18
//
#include "core/common.h"
#include "core/utils.h"
#include "io/log.h"
#include "assetpalette.h"
#include "assetimage.h"
#include "graphics/window.h"
#include "core/game.h"
#include "assetmanager.h"

//Rect pack
#define STB_RECT_PACK_IMPLEMENTATION
#define STBRP_LARGE_RECTS
#include "stb_rect_pack.h"

AssetManager::AssetManager(std::shared_ptr<Game> game): game(game) {
    log = Log::get("Assets");
}

AssetManager::~AssetManager() {
    log->debug("Closing");
    clearAssets();
}

bool AssetManager::addAsset(std::shared_ptr<Asset> asset) {
    if (!asset) {
        error = "Asset to add is null";
        return false;
    }
    const std::string& path = asset->getPath();
    if (assets[path]) {
        error = "Asset already present: '" + path + "'";
        return false;
    }
    assets[path] = asset;
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

std::shared_ptr<Asset> AssetManager::getAsset(const asset_path& path) {
    return assets[path];
}

template <typename T>
std::shared_ptr<T> AssetManager::getAsset(const asset_path& path) {
    return std::dynamic_pointer_cast<T>(assets[path]);
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

void AssetManager::loadAssets() {
    clearAssets();
    log->debug("Loading assets");

    //Scan assets from containers by checking different paths that might contain assets
    std::string assetDir = std::string(GAME_ASSETS_DIR) + DIR_SEP;
    std::list<std::string> assetDirPaths = {
            assetDir,                                                 //Current directory
            Utils::getInstallPath() + assetDir,                       //Installation directory
            Utils::getParentPath(Utils::getInstallPath()) + assetDir, //Parent of installation directory
    };
    for (std::string name : GAME_ASSET_CONTAINER_NAMES) {
        log->debug("Loading '{0}'", name);
        bool found = false;
        for (std::string path : assetDirPaths) {
            found |= scanContainer(path, name);
            if (found || !error.empty()) break;
        }

        //Nothing was found
        if (!found) {
            std::string text = "Error loading game data for '" + name + "' directory or '" + name + ".WD' file\n";
            text += "Check if game files are correctly set and are accessible inside following paths: \n";
            for (std::string path : assetDirPaths) {
                text += path + "\n";
            }
            //Add prev error
            if (!error.empty()) {
                text += error;
            }
            error = text;
        }
    }

    //Scan intermediate assets
    processIntermediates();

    //Refresh the assets
    refreshAssets();

    //Print loaded assets
    //for (std::pair<asset_path, std::shared_ptr<Asset>> pair : assets) log->debug(pair.first);

    log->debug("Has {0} assets", assetsCount);
}

void AssetManager::refreshAssets() {
    unsigned int textureSize = game->getWindow()->getMaxTextureSize();

    //Iterate all assets
    std::vector<AssetImage*> assetImages;
    for (std::pair<asset_path, std::shared_ptr<Asset>> pair : assets) {
        std::shared_ptr<AssetImage> assetImage = std::dynamic_pointer_cast<AssetImage>(pair.second);
        if (assetImage) {
            assetImages.push_back(assetImage.get());
        }
    }

    //Process the images
    processImages(textureSize, textureSize * textureSize, assetImages);
    if (!error.empty()) return;
}

void AssetManager::processImages(unsigned int textureSize, unsigned int batchSize, std::vector<AssetImage*>& assetImages) {
    //Init structures
    stbrp_context context;
    std::vector<stbrp_node> nodes(textureSize * 2);
    std::vector<stbrp_rect> rects(batchSize);

    int retry = 0;
    size_t totalCount = assetImages.size();
    size_t lastSize = 0;
    while (!assetImages.empty() && lastSize != assetImages.size()) {
        lastSize = assetImages.size();

        //Create the base image
        std::shared_ptr<Image> atlasImage = std::make_shared<Image>(Vector2(textureSize));
        error = atlasImage->getError();
        if (!error.empty()) return;

        //Setup the rects and add the index so we know which image does reference
        unsigned int imageCount = (unsigned int) std::min((size_t) batchSize, lastSize);
        for (unsigned int i = 0; i < imageCount; ++i) {
            stbrp_rect& rect = rects.at(i);
            AssetImage* assetImage = assetImages.at(lastSize - i - 1);
            Vector2 imageSize = assetImage->getImageSize();
            rect.id = i;
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
                textureSize //* 2
        );
        stbrp_pack_rects(&context, rects.data(), imageCount);

        //Get the rects that were packed and use them for actual image setup
        for (unsigned int i = 0; i < imageCount; ++i) {
            //Get rectangle
            stbrp_rect& rect = rects.at(i);
            if (rect.was_packed == 0) {
                //Okay, to next round
                retry++;
                continue;
            }
            if (rect.id != i) {
                //This shouldn't happen unless rects become reordered
                error = "Rect id doesn't match index";
                return;
            };
            Rectangle rectangle(rect.x, rect.y, rect.w, rect.h);

            //Fetch and remove asset image from queue
            std::vector<AssetImage*>::iterator it = assetImages.end() - (rect.id + 1);
            AssetImage* assetImage = *it;
            assetImages.erase(it);

            //Create a subset image which inherits the atlas image using the rectangle
            std::shared_ptr<Image> subImage = std::make_shared<Image>(atlasImage, rectangle);
            error = subImage->getError();
            if (!error.empty()) return;

            //Assign the subset image to the asset, this loads the asset data to image
            bool result = assetImage->assignImage(subImage);
            if (!result) {
                error = assetImage->getError();
                if (error.empty()) error = "Error assigning image to asset";
                error += "\nAsset: " + assetImage->getPath();
                return;
            }
        }
    }

    //Done
    log->debug("Packing retry count {0} image count {1}", retry, totalCount);
    if (lastSize != 0) {
        error = "Packing failed for " + std::to_string(lastSize) + " assets";
        return;
    }
}
