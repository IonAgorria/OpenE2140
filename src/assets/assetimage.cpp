//
// Created by Ion Agorria on 21/04/18
//
#include "assetimage.h"

AssetImage::AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
                       const std::shared_ptr<AssetPalette> palette) :
        Asset(path, file, fileOffset, fileSize), palette(palette) {
}

AssetImage::AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        AssetImage(path, file, fileOffset, fileSize, nullptr) {
    palette.get();
}
