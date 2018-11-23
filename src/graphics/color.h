//
// Created by Ion Agorria on 10/11/18
//
#ifndef OPENE2140_COLOR_H
#define OPENE2140_COLOR_H

#include <string>
#include "core/config.h"

/**
 * Struct for RG color
 */
struct ColorRG {
    byte r = 0;
    byte g = 0;

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
    byte r = 0;
    byte g = 0;
    byte b = 0;

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
struct ColorRGBA {
    byte r = 0;
    byte g = 0;
    byte b = 0;
    byte a = 0;

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

#endif //OPENE2140_COLOR_H