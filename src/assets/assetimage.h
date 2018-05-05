//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_ASSETIMAGE_H
#define OPENE2140_ASSETIMAGE_H

#include "assetpalette.h"
#include "asset.h"

/**
 * Asset for image data
 */
class AssetImage : public Asset {
    /**
     * Palette to use if need
     */
    const std::shared_ptr<AssetPalette> palette;

public:
    /**
     * Constructor for 8 bit palette indexed image
     *
     * @param path where this asset is located
     * @param file to use when accessing data
     * @param fileOffset where asset data start in file
     * @param fileSize of asset data in file, 0 for unknown/until end
     * @param palette to use when decoding image data
     */
    AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize, const std::shared_ptr<AssetPalette> palette);

    /**
     * Constructor for 16 bit image
     *
     * @param path where this asset is located
     * @param file to use when accessing data
     * @param fileOffset where asset data start in file
     * @param fileSize of asset data in file, 0 for unknown/until end
     */
    AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);
};

#endif //OPENE2140_ASSETIMAGE_H
