//
// Created by Ion Agorria on 29/04/18
//
#include "core/utils.h"
#include "assetpalette.h"

AssetPalette::AssetPalette(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        Asset(path, file, fileOffset, fileSize) {
}

bool AssetPalette::assignPalette(std::shared_ptr<Palette> assigningPalette) {
    if (assigningPalette) {
        //Seek to initial position
        if (seek(0, true) < 0 || !error.empty()) {
            error = "Error seeking before writing image " + error;
            return false;
        }

        //Read each color and set to palette
        ColorRGBA colorRGBA;
        for (int i = 0; i < ASSET_PALETTE_COUNT; ++i) {
            //Load RGB and set to RGBA
            ColorRGB colorRGB;
            if (!readAll(colorRGB)) {
                return false;
            }
            colorRGBA.setRGB(colorRGB);

            //Handle first color as transparent and the rest as opaque
            bool isAlphaColor = i == 0 && colorRGB.r == 0 && colorRGB.g == 0 && colorRGB.b == 0;
            colorRGBA.a = isAlphaColor ? 0 : 0xFF;

            //Set it to provided palette
            if (!assigningPalette->setColor(i, colorRGBA)) {
                error = assigningPalette->getError();
                break;
            }
        }

        //Update the palette
        if (error.empty()) {
            if (!assigningPalette->updateTexture()) {
                error = assigningPalette->getError();
            }
        }
    }

    //Assign the palette to this asset
    if (error.empty()) {
        if (assigningPalette) {
            this->palette = assigningPalette;
        } else {
            this->palette.reset();
        }
    }

    //Return result
    return error.empty();
}

std::shared_ptr<Palette> AssetPalette::getPalette() const {
    return palette;
}

std::string AssetPalette::toString() const {
    return "AssetPalette(" + toStringContent() + ")";
}