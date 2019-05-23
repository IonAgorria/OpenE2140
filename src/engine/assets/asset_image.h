//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_ASSETIMAGE_H
#define OPENE2140_ASSETIMAGE_H

#include "math/vector2.h"
#include "asset.h"

/**
 * Asset for image data
 */
class AssetPalette;
class Image;
class AssetImage : public Asset {
private:
    /**
     * Image size
     */
    const Vector2 imageSize;

    /**
     * Asset containing palette data for this image
     */
    const std::shared_ptr<AssetPalette> assetPalette;

    /**
     * Image containing this asset's data
     */
    std::shared_ptr<Image> image;

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
        const Vector2& size, const std::shared_ptr<AssetPalette> assetPalette);

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
     * Asset destructor
     */
    virtual ~AssetImage() = default;

    /**
     * @return image size
     */
    const Vector2& getImageSize() const;

    /**
     * Writes this asset image content to provided image and set this image in the asset
     *
     * @param image to write this asset content to
     * @return true if success
     */
    bool assignImage(std::shared_ptr<Image> assigningImage);

    /**
     * @return palette asset related to this image asset if any
     */
    std::shared_ptr<AssetPalette> getAssetPalette() const;

    /**
     * @return image containing data for this image asset
     */
    std::shared_ptr<Image> getImage() const;

    /**
     * @return string version of this asset
     */
    std::string toString() const override;

    /**
    * @return string content of this asset
    */
    std::string toStringContent() const override;
};

#endif //OPENE2140_ASSETIMAGE_H
