//
// Created by Ion Agorria on 21/04/19
//
#include <unordered_map>
#include <forward_list>
#include <src/engine/core/utils.h>

#include "io/file.h"
#include "assets/asset_manager.h"
#include "assets/asset.h"
#include "assets/asset_palette.h"
#include "assets/asset_image.h"
#include "asset_processor_datpal.h"

void AssetProcessorDatPal::processIntermediates() {
    //Cached assets
    std::unordered_map<asset_path, std::shared_ptr<AssetPalette>> assetPalettes;
    std::forward_list<asset_path> datPaths;

    //Iterate all assets
    for (const std::pair<const asset_path, std::unique_ptr<Asset>>& pair : manager->getAssets()) {
        //Get the "extension" of asset
        asset_path assetPath = pair.first;
        std::string::size_type size = assetPath.size();
        if (4 > size) {
            continue;
        }
        std::string ext = assetPath.substr(size - 4, 4);

        //Handle special extensions
        Asset* asset = pair.second.get();
        if (ext == ".PAL") {
            //Create palette asset and store it
            std::shared_ptr<AssetPalette> assetPalette = std::make_shared<AssetPalette>(
                    assetPath, asset->getFile(), asset->offset(), asset->size()
            );
            assetPalettes[assetPath] = assetPalette;
        } else if (ext == ".DAT") {
            //Store asset path for later if they are images
            bool isImage = !Utils::startsWith(assetPath, "LEVEL/DATA");
            if (isImage) {
                datPaths.push_front(assetPath);
            }
        }
    }

    //Iterate images paths
    for (asset_path assetPath : datPaths) {
        //Get the asset
        std::string::size_type size = assetPath.size();
        asset_path imagePath = assetPath.substr(0, size - 4);
        Asset* asset = manager->getAsset(assetPath);

        //Check if there is palette asset under same name
        asset_path palettePath = imagePath + ".PAL";
        std::shared_ptr<AssetPalette> assetPalette = assetPalettes[palettePath];
        if (!assetPalette) {
            error = assetPath + " doesn't have palette counterpart";
            return;
        }

        //Pass the image size struct to fill it
        size_t readSize = sizeof(SSize16) + 2;
        Vector2 imageSize;
        SSize16 imageSizeStruct;
        if (!asset->readAll(imageSizeStruct)) {
            error = "Error reading '" + assetPath + "' image size\n" + asset->getError();
            return;
        }
        imageSize.set(imageSizeStruct.width, imageSizeStruct.height);

        //Skip 2 bytes
        asset->seek(2);
        error = asset->getError();
        if (!error.empty()) {
            error = "Error reading '" + assetPath + "' when seeking " + asset->getError();
            return;
        }

        //Create image asset with the palette indexes data and store it
        std::unique_ptr<AssetImage> assetImage = std::make_unique<AssetImage>(
                imagePath, asset->getFile(), asset->offset() + readSize, asset->size() - readSize, imageSize, assetPalette
        );
        if (!manager->addAsset(std::move(assetImage))) {
            error = "Couldn't create asset from processed asset\n" + error;
            return;
        }

        //Remove the old assets
        if (!manager->removeAsset(assetPath) || !manager->removeAsset(palettePath)) {
            error = "Couldn't remove processed asset\n" + error;
            return;
        }
    }
}