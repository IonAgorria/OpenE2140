//
// Created by Ion Agorria on 21/04/18
//
#include <core/utils.h>
#include "assetimage.h"
#include "assetpalette.h"

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
        if (imagePixelsCount * 2 != static_cast<size_t>(size())) {
            error = "Asset size doesn't match image size";
            return false;
        }

        //Buffer to store the raw image after palette lookup
        std::unique_ptr<byteArray> buffer = Utils::createBuffer(imagePixelsCount * 4);

        //Convert each palette color index into RGB
        for (unsigned int i = 0; i < imagePixelsCount; i++) {
            //Get color index
            byte colorIndex;
            if (!readAll(colorIndex)) {
                if (error.empty()) {
                    error = "Error reading color index";
                }
                return false;
            }

            //Get color from palette
            AssetPalette::Color color;
            if (!palette->getColor(colorIndex, color)) {
                error = palette->getError();
                if (error.empty()) {
                    error = "Error reading palette color";
                }
                return false;
            }

            //Store color values in buffer
            int bufferI = i * 4;
            buffer[bufferI + 1] = color.B; //B
            buffer[bufferI + 2] = color.G; //G
            buffer[bufferI + 3] = color.R; //R
        }

        //Set each alpha value
        for (unsigned int i = 0; i < imagePixelsCount; i++) {
            byte alpha;
            if (!readAll(alpha)) {
                if (error.empty()) {
                    error = "Error reading alpha";
                }
                return false;
            }
            buffer[i * 4] = alpha;
        }

        //Load buffer
        if (error.empty()) {
            result = image.loadFromRGBA8888(buffer.get());
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
