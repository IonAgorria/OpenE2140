//
// Created by Ion Agorria on 29/04/18
//
#include "assetpalette.h"

AssetPalette::AssetPalette(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        Asset(path, file, fileOffset, fileSize) {
}

bool AssetPalette::checkIndex(int index) {
    return 0 <= index && index < PALETTE_COUNT;
}

bool AssetPalette::getColor(int index, ColorRGB& color) {
    //Check index
    if (!checkIndex(index)) {
        error = "Index out of bounds: " + std::to_string(index);
        return false;
    }

    //Lookup the color
    const size_t amount = sizeof(ColorRGB);
    long pos = seek(index * amount, true);
    if (0 > pos) {
        return false;
    }

    //Read the color
    return readAll(color);
}

bool AssetPalette::setColor(int index, ColorRGB& color) {
    //Check index
    if (!checkIndex(index)) {
        error = "Index out of bounds: " + std::to_string(index) + " color " + color.toString();
        return false;
    }

    //Not used
    return false;
}

std::string AssetPalette::toString() const {
    return "AssetPalette(" + toStringContent() + ")";
}