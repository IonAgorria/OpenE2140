//
// Created by Ion Agorria on 21/04/18
//
#include "assetimage.h"

AssetImage::AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
                       const Vector2& size, const std::shared_ptr<AssetPalette> palette) :
        Asset(path, file, fileOffset, fileSize), imageSize(size), palette(palette) {
}

AssetImage::AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
                       const Vector2& size) :
        AssetImage(path, file, fileOffset, fileSize, size, nullptr) {
}

std::string AssetImage::toString() {
    return "AssetImage(" + toStringContent() + ")";
}

std::string AssetImage::toStringContent() {
    return Asset::toStringContent()
        + " Palette: " + (palette ? palette->toString() : "null")
    ;
}