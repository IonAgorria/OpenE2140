//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSETPALETTE_H
#define OPENE2140_ASSETPALETTE_H

#include "graphics/palette.h"
#include "asset.h"

/** Palette color count */
#define ASSET_PALETTE_COUNT 256

/**
 * Palette information asset
 */
class AssetPalette : public Asset {
private:
    /**
     * Asset containing palette data for this image
     */
    const std::shared_ptr<AssetPalette> assetPalette;

    /**
     * Palette containing this asset's data
     */
    std::shared_ptr<Palette> palette;

public:
    /**
     * Constructor for palette asset
     *
     * @param path where this asset is located
     * @param file to use when accessing data
     * @param fileOffset where asset data start in file
     * @param fileSize of asset data in file, 0 for unknown/until end
     */
    AssetPalette(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * Writes this asset palette content to palette and set this image in the asset
     *
     * @param palette to write this asset content to
     * @return true if success
     */
    bool assignPalette(std::shared_ptr<Palette> assigningPalette);

    /**
     * @return palette containing data for this palette asset
     */
    std::shared_ptr<Palette> getPalette() const;

    /**
     * @return string version of this asset
     */
    std::string toString() const override;
};

#endif //OPENE2140_ASSETPALETTE_H
