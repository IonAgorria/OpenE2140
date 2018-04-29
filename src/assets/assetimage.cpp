//
// Created by Ion Agorria on 21/04/18
//
#include "assetimage.h"

AssetImage::AssetImage(const std::string& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        Asset(path, file, fileOffset, fileSize) {
}

AssetImage::AssetImage(const std::string& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
                       const std::shared_ptr<AssetPalette> palette) :
        Asset(path, file, fileOffset, fileSize), palette(palette) {
}
