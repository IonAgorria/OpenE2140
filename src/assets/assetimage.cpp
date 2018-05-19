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
    size_t imagePixelsCount = (unsigned) imageSize.x * imageSize.y;

    //Seek to initial position
    if (seek(0, true) < 0 || !error.empty()) {
        error = "Error seeking before writing image " + error;
        return false;
    }

    //Handle according to asset image type (paletted or raw)
    if (palette) {
        //Buffer to store the raw image after palette lookup
        std::unique_ptr<byteArray> buffer = Utils::createBuffer(imagePixelsCount * 3);

        //Convert each palette color index into RGB
        for (unsigned int i = 0; i < imagePixelsCount; i++) {
            //Get color index for this index
            byte colorIndex;
            if (!readAll(colorIndex)) {
                if (error.empty()) {
                    error = "Error reading color index";
                }
                break;
            }

            //Get color from palette
            AssetPalette::Color color;
            if (0 >= palette->getColor(colorIndex, color)) {
                error = palette->getError();
                if (error.empty()) {
                    error = "Error reading palette color";
                }
                break;
            }

            //Store color values in buffer
            int bufferI = i * 3;
            buffer[bufferI]     = color.R;
            buffer[bufferI + 1] = color.G;
            buffer[bufferI + 2] = color.B;
        }

        //Load buffer
        if (error.empty()) {
            result = image.loadFromRGB888(buffer.get(), nullptr);
            error = image.getError();
        } else {
            result = false;
        }
    } else {
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
