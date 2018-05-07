//
// Created by Ion Agorria on 29/04/18
//
#include "assetpalette.h"

AssetPalette::AssetPalette(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        Asset(path, file, fileOffset, fileSize) {
}

long AssetPalette::getColor(int index, AssetPalette::Color& color) {
    //Check index
    if (0 > index || index >= PALETTE_COUNT) {
        error = "Index out of bounds: " + std::to_string(index);
        return -1;
    }

    //Lookup the color
    const size_t amount = sizeof(Color);
    long pos = seek(index * amount, true);
    if (0 <= pos) {
        //Set value
        if (read(&color, amount) != amount) {
            return 0;
        }
    }

    //Return result
    return pos;
}

std::string AssetPalette::toString() {
    return "AssetPalette(" + toStringContent() + ")";
}