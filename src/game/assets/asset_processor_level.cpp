//
// Created by Ion Agorria on 21/04/19
//

#include <forward_list>
#include "assets/asset_manager.h"
#include "asset_processor_level.h"
#include "asset_world_level.h"

AssetWorldLevel::AssetWorldLevel(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        AssetWorld(path, file, fileOffset, fileSize) {
}

void AssetProcessorLevel::processIntermediates() {
    std::unordered_map<asset_path_t,Asset*> assets;

    //Iterate all assets
    for (const std::pair<const asset_path_t, std::unique_ptr<Asset>>& pair : manager->getAssets()) {
        asset_path_t assetPath = pair.first;

        //Check if its level
        if (Utils::startsWith(assetPath, "LEVEL/DATA/LEVEL") && !Utils::endsWith(assetPath, ".INI")) {
            assets[assetPath] = pair.second.get();
        }
    }

    for (std::pair<asset_path_t,Asset*> pair : assets) {
        Asset* asset = pair.second;

        //Get INI of level if available
        //TODO use INI asset to read mission description
        std::string::size_type assetSize = pair.first.size();
        asset_path_t assetPathBase = pair.first.substr(0, assetSize - 4);
        Asset* ini = manager->getAsset(assetPathBase + ".INI");

        //Create palette asset and store it
        std::unique_ptr<AssetWorldLevel> assetWorld = std::make_unique<AssetWorldLevel>(
                assetPathBase, asset->getFile(), asset->offset(), asset->size()
        );

        //Add asset
        if (!manager->addAsset(std::move(assetWorld))) {
            error = "Couldn't create asset from processed asset\n" + manager->getError();
            return;
        }

        //Remove the old assets
        if (!manager->removeAsset(pair.first)) {
            error = "Couldn't remove processed level asset\n" + manager->getError();
            return;
        }
        if (ini && !manager->removeAsset(ini->getPath())) {
            error = "Couldn't remove processed level INI asset\n" + manager->getError();
            return;
        }
    }
}