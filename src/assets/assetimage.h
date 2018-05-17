//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_ASSETIMAGE_H
#define OPENE2140_ASSETIMAGE_H

#include <core/math/vector2.h>
#include "assetpalette.h"
#include "asset.h"

/** 8 bit index palette image */
#define TYPE_IMAGE_8_INDEXED 1
/** Raw 16 bits per color image */
#define TYPE_IMAGE_16_RAW 2
/** Sprite image with scanlines and segments */
#define TYPE_IMAGE_SEGMENTED 9

/**
 * Asset for image data
 */
class AssetImage : public Asset {
    /**
     * Palette to use if need
     */
    const std::shared_ptr<AssetPalette> palette;

    /**
     * Image size
     */
     const Vector2 imageSize;

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
    AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
        const Vector2& size, const std::shared_ptr<AssetPalette> palette);

    /**
     * Constructor for 16 bit image
     *
     * @param path where this asset is located
     * @param file to use when accessing data
     * @param fileOffset where asset data start in file
     * @param fileSize of asset data in file, 0 for unknown/until end
     */
    AssetImage(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize,
       const Vector2& size);

    /**
     * @return string version of this asset
     */
    std::string toString() override;

    /**
    * @return string content of this asset
    */
    std::string toStringContent() override;
};

#endif //OPENE2140_ASSETIMAGE_H
