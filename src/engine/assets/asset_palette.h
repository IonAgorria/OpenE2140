//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSETPALETTE_H
#define OPENE2140_ASSETPALETTE_H

#include "engine/graphics/palette.h"
#include "asset.h"

/** Palette color count */
#define ASSET_PALETTE_COUNT 256

/**
 * Palette information asset
 */
class AssetPalette : public Asset {
private:
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
    AssetPalette(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * Asset destructor
     */
    virtual ~AssetPalette() = default;

    /**
     * Macros
     */
    TYPE_NAME_OVERRIDE(AssetPalette)

    /**
     * Writes this asset palette content to palette and set this image in the asset
     *
     * @param palette to write this asset content to
     * @return true if success
     */
    bool assignPalette(const std::shared_ptr<Palette>& assigningPalette);

    /**
     * @return palette containing data for this palette asset
     */
    std::shared_ptr<Palette> getPalette() const;
};

#endif //OPENE2140_ASSETPALETTE_H
