//
// Created by Ion Agorria on 10/11/18
//
#ifndef OPENE2140_COLOR_H
#define OPENE2140_COLOR_H

#include "SDL_pixels.h"
#include "engine/core/common.h"

/**
 * Struct for RG color
 */
struct ColorRG {
    byte_t r = 0;
    byte_t g = 0;

    /**
     * Sets this color from other color
     */
    void setRGB(const ColorRG& color) {
        r = color.r;
        g = color.g;
    }

    /**
     * @return string version of this color
     */
    std::string toString() const {
        return " R " + std::to_string(r) + " G " + std::to_string(g);
    }
};

/**
 * Struct for RGB color
 */
struct ColorRGB {
    byte_t r = 0;
    byte_t g = 0;
    byte_t b = 0;

    /**
     * Sets this color from other color
     */
    void setRGB(const ColorRGB& color) {
        r = color.r;
        g = color.g;
        b = color.b;
    }

    /**
     * @return string version of this color
     */
    std::string toString() const {
        return " R " + std::to_string(r) + " G " + std::to_string(g) + " B " + std::to_string(b);
    }
};

/**
 * Struct for RGBA color
 */
struct ColorRGBA: public SDL_Color {
    /**
     * Sets this color from other color
     */
    void setRGB(const ColorRGB& color) {
        r = color.r;
        g = color.g;
        b = color.b;
    }

    /**
     * Sets this color from other color
     */
    void setRGBA(const ColorRGBA& color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }

    /**
     * @return string version of this color
     */
    std::string toString() const {
        return " R " + std::to_string(r) + " G " + std::to_string(g)
             + " B " + std::to_string(b) + " A " + std::to_string(a);
    }
};

/**
 * Namespace for color constants
 */
namespace Color {
    const ColorRGBA CLEAR           {0x00, 0x00, 0x00, 0x00};
    const ColorRGBA BLACK           {0x00, 0x00, 0x00, 0xFF};
    const ColorRGBA RED             {0xFF, 0x00, 0x00, 0xFF};
    const ColorRGBA GREEN           {0x00, 0xFF, 0x00, 0xFF};
    const ColorRGBA BLUE            {0xFF, 0x00, 0x00, 0xFF};
    const ColorRGBA WHITE           {0xFF, 0xFF, 0xFF, 0xFF};
    const ColorRGBA GREY            {0x80, 0x80, 0x80, 0xFF};
    const ColorRGBA DEBUG_WORLD     {0xA0, 0xA0, 0xA0, 0x20};
    const ColorRGBA DEBUG_ENTITIES  {0xFF, 0x00, 0x00, 0x40};
}

#endif //OPENE2140_COLOR_H
