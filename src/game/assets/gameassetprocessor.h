//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_GAMEASSETPROCESSOR_H
#define OPENE2140_GAMEASSETPROCESSOR_H

#include "core/types.h"
#include "assets/iassetprocessor.h"

//TODO move pallete stuff somewhere else
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

/** 8 bit index palette image */
#define TYPE_IMAGE_8_INDEXED 1
/** Raw 16 bits per color image */
#define TYPE_IMAGE_16_RAW 2
/** Sprite image with scanlines and segments */
#define TYPE_IMAGE_SEGMENTED 9

class Asset;
class GameAssetProcessor: public IAssetProcessor {
private:
    /**
     * Each WD container file record struct
     */
    struct WDFileRecord {
        unsigned int fileOffset;
        unsigned int fileSize;
        unsigned int unused1;
        unsigned int unused2;
        unsigned int checkSum;
        unsigned int nameOffset;
    };

    /**
     * MIX file header struct
     */
    struct MIXHeader {
        unsigned int unused;
        unsigned int streamsCount;
        unsigned int streamsOffset;
        unsigned int palettesCount;
        unsigned int palettesFirstIndex;
        unsigned int palettesOffset;
    };

    /**
     * Image size struct with shorts
     */
    struct ImageSize16 {
        unsigned short width;
        unsigned short height;
    };

    /**
     * Segmented image header
     */
    struct SegmentedImageHeader {
        unsigned int width;
        unsigned int height;
        unsigned int dataBlockSize;
        unsigned int scanLinesCount;
        unsigned int segmentBlockSize;
        unsigned int unknown6;
        unsigned int unknown7;
        unsigned int unknown8;
        unsigned int unknown9;
    };

    /**
     * Segmented image segment
     */
    struct SegmentedImageSegment {
        byte padding;
        byte width;
    };

    virtual bool scanContainer(const std::string& path, const std::string& name) override;

    /**
     * Scans assets from WD file container and stores in manager
     *
     * @param path to use as assets root
     * @param name of container
     * @return amount of loaded assets or -1 if error occurred
     */
    int scanContainerWD(const std::string& path, const std::string& name);

    /**
     * Processes the content of a MIX asset for more assets
     *
     * @param asset containing MIX
     * @return amount of added assets or -1 if error
     */
    int processIntermediateMIX(Asset* asset);

    virtual void processIntermediates() override;
};

#endif //OPENE2140_GAMEASSETPROCESSOR_H
