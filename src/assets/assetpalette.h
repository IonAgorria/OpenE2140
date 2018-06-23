//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSETPALETTE_H
#define OPENE2140_ASSETPALETTE_H

#include "core/graphics/palette.h"
#include "asset.h"

/** Palette color count */
#define PALETTE_COUNT 256
/** Size of each palette in bytes */
#define PALETTE_RGB_SIZE PALETTE_COUNT * sizeof(Palette::ColorRGB)

//Unit movement dependent colors (threads/wheels/jets)
#define PALETTE_UNIT_MOVEMENT0 0xF0
#define PALETTE_UNIT_MOVEMENT1 0xF1
#define PALETTE_UNIT_MOVEMENT2 0xF2
#define PALETTE_UNIT_MOVEMENT3 0xF3
//Unit weapon flame
#define PALETTE_UNIT_FLAME0 0xF4
#define PALETTE_UNIT_FLAME1 0xF5
#define PALETTE_UNIT_FLAME2 0xF6
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
class AssetPalette : public Asset, public Palette {
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

    bool checkIndex(int index) override;

    bool getColor(int index, ColorRGB& color) override;

    bool setColor(int index, ColorRGB& color) override;

    /**
     * @return string version of this asset
     */
    std::string toString() const override;
};

#endif //OPENE2140_ASSETPALETTE_H
