//
// Created by Ion Agorria on 2/06/19
//
#ifndef OPENE2140_CONSTANTS_H
#define OPENE2140_CONSTANTS_H

#define MONEY_PER_CONTAINER 200
#define TILE_SIZE 64
#define SPRITE_ROTATION_ANGLES 9

/**
 * Entity kind IDs
 */
#define ENTITY_KIND_OBJECT 0
#define ENTITY_KIND_UNIT 1
#define ENTITY_KIND_BUILDING 2
#define ENTITY_KIND_ATTACHMENT 3
#define ENTITY_KIND_PROJECTILE 4
#define ENTITY_KIND_EFFECT 5

/**
 * Constants for palette indexes with special meaning
 */
#define PALETTE_MAX_INDEX 0xFF
//Player coloring entries
#define PALETTE_PLAYER 0xF8
#define PALETTE_PLAYER_AMOUNT 5
//Shadow
#define PALETTE_SHADOW 0xFE

#define ENTITY_OBJECTS_VARIANTS {"0", "1", "2", "3", "4", "5", "6"}

/**
 * Namespace for color constants
 */
#include "engine/graphics/color.h"
namespace Color {
    const ColorRGBA SHADOW_MAIN            {{0x00, 0x00, 0x00, 0x40}};
    const ColorRGBA SHADOW_EXTRA           {{0x00, 0x00, 0x00, 0x20}};
    const ColorRGBA MAP_ORE_COLOR          {{0x80, 0x80, 0x00, 0xFF}};
    /**
     * HSV values to mix with player base color
     * Take must be care to maintain the gradual change between palette colors and avoid banding or oversaturation
     */
    const ColorHSV PLAYER[] {
            {0, 1.00f, 0.30f},
            {0, 1.00f, 0.40f},
            {0, 1.00f, 0.50f},
            {0, 1.00f, 0.60f},
            {0, 1.00f, 0.70f}
    };
}

#endif //OPENE2140_CONSTANTS_H
