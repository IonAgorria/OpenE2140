//
// Created by Ion Agorria on 21/04/19
//

#include <forward_list>
#include "assets/asset_manager.h"
#include "assets/asset.h"
#include "asset_processor_level.h"

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
        //Get INI of level if available
        std::string::size_type assetSize = pair.first.size();
        asset_path_t assetPathBase = pair.first.substr(0, assetSize - 4);
        Asset* ini = manager->getAsset(assetPathBase + ".INI");

        //Process it
        processLevel(pair.second, ini);

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

void AssetProcessorLevel::processLevel(Asset* level, Asset* ini) {
    size_t size = 32;
    std::unique_ptr<byte_array_t> buffer = Utils::createBuffer(size);
    level->read(buffer.get(), size);
    Log::get()->debug(buffer.get());
    if (ini) {

    }

}