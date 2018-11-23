//
// Created by Ion Agorria on 21/04/18
//
#include <core/utils.h>
#include "assetimage.h"
#include "assetpalette.h"
#include "graphics/color.h"

AssetImage::AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
                       const Vector2& size, const std::shared_ptr<AssetPalette> palette) :
        Asset(path, file, fileOffset, fileSize), imageSize(size), palette(palette) {
}

AssetImage::AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
                       const Vector2& size) :
        AssetImage(path, file, fileOffset, fileSize, size, nullptr) {
}

const Vector2& AssetImage::getImageSize() const {
    return imageSize;
}

bool AssetImage::writeImage(Image& image) {
    bool result = 0;
    size_t imagePixelsCount = static_cast<size_t>(imageSize.x * imageSize.y);

    //Seek to initial position
    if (seek(0, true) < 0 || !error.empty()) {
        error = "Error seeking before writing image " + error;
        return false;
    }

    //Handle according to asset image type (paletted or raw)
    if (palette) {
        //Check size
        if (imagePixelsCount != static_cast<size_t>(size())) {
            error = "Asset size doesn't match image size";
            return false;
        }

        //Create buffer, read asset into it and load to image
        std::unique_ptr<byteArray> buffer = Utils::createBuffer(imagePixelsCount);
        if (readAll(buffer.get(), imagePixelsCount * 2)) {
            result = 0;//image.loadFromI8(buffer.get(), nullptr);
            error = image.getError();
        } else {
            result = false;
        }
    } else {
        //Check size
        if (imagePixelsCount * 2 != static_cast<size_t>(size())) {
            error = "Asset size doesn't match image size";
            return false;
        }

        //Create buffer, read asset into it and load to image
        std::unique_ptr<byteArray> buffer = Utils::createBuffer(imagePixelsCount * 2);
        if (readAll(buffer.get(), imagePixelsCount * 2)) {
            result = image.loadFromRGB565(buffer.get(), nullptr);
            error = image.getError();
        } else {
            result = false;
        }
    }

    //Return result
    return result && error.empty();
}

std::string AssetImage::toString() const {
    return "AssetImage(" + toStringContent() + ")";
}

std::string AssetImage::toStringContent() const {
    return Asset::toStringContent()
        + " Palette: " + (palette ? palette->toString() : "null")
    ;
}
