//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSETPALETTE_H
#define OPENE2140_ASSETPALETTE_H

#include "graphics/palette.h"
#include "asset.h"

/** Palette color count */
#define ASSET_PALETTE_COUNT 256

//Unit movement dependent colors (threads/wheels/jets)
#define PALETTE_UNIT_MOVEMENT0 0xF0
#define PALETTE_UNIT_MOVEMENT1 0xF1
#define PALETTE_UNIT_MOVEMENT2 0xF2
#define PALETTE_UNIT_MOVEMENT3 0xF3
//Unit weapon fire
#define PALETTE_UNIT_FIRE0 0xF4
#define PALETTE_UNIT_FIRE1 0xF5
#define PALETTE_UNIT_FIRE2 0xF6
//Unit light
#define PALETTE_UNIT_LIGHT0 0xF7
//Unit player coloring
#define PALETTE_UNIT_PLAYER0 0xF8
#define PALETTE_UNIT_PLAYER1 0xF9
#define PALETTE_UNIT_PLAYER2 0xFA
#define PALETTE_UNIT_PLAYER3 0xFB
#define PALETTE_UNIT_PLAYER4 0xFC
#define PALETTE_UNIT_PLAYER5 0xFD
//Unit shadow
#define PALETTE_UNIT_SHADOW0 0xFE

//Building lights to be set as black when has no power (shipyard doesn't have so not set)
#define PALETTE_BUILDING_LIGHT0 0xF4
#define PALETTE_BUILDING_LIGHT1 0xF5
#define PALETTE_BUILDING_LIGHT2 0xF6
#define PALETTE_BUILDING_LIGHT3 0xF7
//Building player coloring
#define PALETTE_BUILDING_PLAYER0 0xF8
#define PALETTE_BUILDING_PLAYER1 0xF9
#define PALETTE_BUILDING_PLAYER2 0xFA
#define PALETTE_BUILDING_PLAYER3 0xFB
#define PALETTE_BUILDING_PLAYER4 0xFC
//Building shadow tones
#define PALETTE_BUILDING_SHADOW0 0xFD
#define PALETTE_BUILDING_SHADOW1 0xFE

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
