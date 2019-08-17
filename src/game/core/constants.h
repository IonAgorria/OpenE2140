//
// Created by Ion Agorria on 2/06/19
//
#ifndef OPENE2140_CONSTANTS_H
#define OPENE2140_CONSTANTS_H

#define MONEY_PER_CONTAINER 200
#define CAMERA_SPEED 10
#define TILE_SIZE 64

/**
 * Entity kind IDs
 */
#define ENTITY_KIND_OBJECT 0
#define ENTITY_KIND_UNIT 1
#define ENTITY_KIND_BUILDING 2
#define ENTITY_KIND_PROJECTILE 3
#define ENTITY_KIND_EXPLOSION 4

/**
 * Namespace for color constants
 */
#include "engine/graphics/color.h"
namespace Color {
    const ColorRGBA SHADOW_MAIN            {0x00, 0x00, 0x00, 0x60};
    const ColorRGBA SHADOW_EXTRA           {0x00, 0x00, 0x00, 0x40};
    const ColorHSV PLAYER[] {
            {120, 1.0f, 0.3f},
            {96, 1.0f, 0.5f},
            {80, 1.0f, 0.6f},
            {80, 1.0f, 0.75f},
            {80, 1.0f, 0.9f}
    };
}

#endif //OPENE2140_CONSTANTS_H
