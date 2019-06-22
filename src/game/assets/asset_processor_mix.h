//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_GAMEASSETPROCESSOR_H
#define OPENE2140_GAMEASSETPROCESSOR_H

#include "engine/core/types.h"
#include "engine/assets/asset_processor.h"

/** 8 bit index palette image */
#define TYPE_IMAGE_8_INDEXED 1
/** Raw 16 bits per color image */
#define TYPE_IMAGE_16_RAW 2
/** Sprite image with scanlines and segments */
#define TYPE_IMAGE_SEGMENTED 9

class Asset;

/**
 * Handles the decoding of MIX image packs into standalone image assets
 */
class AssetProcessorMIX: public IAssetProcessor {
private:
    /**
     * MIX file header struct
     */
    struct mix_header_t {
        unsigned int unused;
        unsigned int streamsCount;
        unsigned int streamsOffset;
        unsigned int palettesCount;
        unsigned int palettesFirstIndex;
        unsigned int palettesOffset;
    };

    /**
     * Segmented image header
     */
    struct segmented_image_header_t {
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
    struct segmented_image_segment_t {
        byte_t padding;
        byte_t width;
    };

    /**
     * Processes the content of a MIX asset for more assets
     *
     * @param asset containing MIX
     */
    void processIntermediateMIX(Asset* asset);

    virtual void processIntermediates() override;
};

#endif //OPENE2140_GAMEASSETPROCESSOR_H
