//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_PALETTE_H
#define OPENE2140_PALETTE_H

#include <string>
#include <unordered_map>
#include <memory>
#include "core/config.h"
#include "core/errorpossible.h"

/**
 * Palette interface
 */
class Palette {
public:
    /**
     * Struct for each color
     */
    struct ColorRGB {
        byte r;
        byte g;
        byte b;

        /**
         * Sets this color from other color
         */
        void set(const ColorRGB& color) {
            r = color.r;
            g = color.g;
            b = color.b;
        }

        /**
         * @return string version of this color
         */
        std::string toString() const {
            return " R " + std::to_string(r)
                 + " G " + std::to_string(g)
                 + " B " + std::to_string(b)
            ;
        }
    };

    /**
     * Checks index of palette is valid
     *
     * @param index to check
     * @return true if OK
     */
    virtual bool checkIndex(int index) {
        return 0 <= index;
    }

    /**
     * Gets the color value at index in this palette
     *
     * @param index to look
     * @param color struct to fill
     * @return true if OK
     */
    virtual bool getColor(int index, ColorRGB& color) = 0;

    /**
     * Sets the color value at index in this palette
     *
     * @param index to set
     * @param color struct to use
     * @return true if OK
     */
    virtual bool setColor(int index, ColorRGB& color) = 0;
};

/**
 * Simple palette implementation
 */
class MapPalette : public Palette, public ErrorPossible {
protected:
    /**
     * Internal palette
     */
    std::unordered_map<int, ColorRGB> colors;

public:
    /**
     * Constructor for empty palette
     */
    MapPalette();

    /**
     * Constructor for preset palette
     */
    MapPalette(std::unordered_map<int, ColorRGB> colors);

    bool getColor(int index, ColorRGB& color) override;

    bool setColor(int index, ColorRGB& color) override;
};

#endif //OPENE2140_PALETTE_H
