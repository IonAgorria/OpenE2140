//
// Created by Ion Agorria on 21/04/19
//

#include <forward_list>
#include "assets/asset_manager.h"
#include "assets/asset.h"
#include "asset_processor_level.h"

void AssetProcessorLevel::processIntermediates() {
    std::forward_list<Asset*> assets;

    //Iterate all assets
    for (const std::pair<const asset_path, std::unique_ptr<Asset>>& pair : manager->getAssets()) {
        //Get the "extension" of asset
        asset_path assetPath = pair.first;
        std::string::size_type size = assetPath.size();
        if (4 > size) {
            continue;
        }

        //Check if its level
        if (assetPath.find("LEVEL/DATA/", 0) == 0) {
            assets.push_front(pair.second.get());
        }
    }

    for (Asset* asset : assets) {
        //Log::get("Main")->debug(asset->getPath());
    }
}