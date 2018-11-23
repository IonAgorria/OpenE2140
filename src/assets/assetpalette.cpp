//
// Created by Ion Agorria on 29/04/18
//
#include "assetpalette.h"

AssetPalette::AssetPalette(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        Asset(path, file, fileOffset, fileSize) {
}

unsigned long AssetPalette::length() const {
    return ASSET_PALETTE_COUNT;
}

bool AssetPalette::getColorVirtual(unsigned int index, ColorRGBA& color) {
    //Lookup the color
    const size_t amount = sizeof(ColorRGB);
    long pos = seek(index * amount, true);
    if (0 > pos) {
        return false;
    }

    //Read the color
    ColorRGB colorRGB;
    if (!readAll(colorRGB)) {
        return false;
    }
    color.setRGB(colorRGB);
    return true;
}

bool AssetPalette::setColorVirtual(unsigned int index, ColorRGBA& color) {
    //Not used
    if (0 < index || color.a == 0) {}
    return false;
}

std::string AssetPalette::toString() const {
    return "AssetPalette(" + toStringContent() + ")";
}