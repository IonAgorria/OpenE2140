//
// Created by Ion Agorria on 21/04/19
//

#include <forward_list>
#include "engine/assets/asset_manager.h"
#include "asset_processor_level.h"
#include "asset_level_game.h"

void AssetProcessorLevel::processIntermediates() {
    std::unordered_map<asset_path_t,Asset*> assets;

    //Iterate all assets
    for (auto& pair : manager->getAssets()) {
        asset_path_t assetPath = pair.first;

        //Check if its level
        if (Utils::startsWith(assetPath, "LEVEL/DATA/LEVEL") && !Utils::endsWith(assetPath, ".INI")) {
            assets[assetPath] = pair.second.get();
        }
    }

    for (auto& pair : assets) {
        Asset* asset = pair.second;

        //Get INI of level if available
        //TODO use INI asset to read mission description
        std::string::size_type assetSize = pair.first.size();
        asset_path_t assetPathBase = pair.first.substr(0, assetSize - 4);
        Asset* ini = manager->getAsset(assetPathBase + ".INI");

        //Create palette asset and store it
        std::unique_ptr<AssetLevelGame> assetWorld = std::make_unique<AssetLevelGame>(
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