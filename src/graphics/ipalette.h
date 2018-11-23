//
// Created by Ion Agorria on 13/11/18
//
#ifndef OPENE2140_IPALETTE_H
#define OPENE2140_IPALETTE_H

#include "color.h"
#include "core/errorpossible.h"

/**
 * Palette interface
 */
class IPalette: public IErrorPossible {
private:
    /**
     * Returns the number of colors in palette
     *
     * @param index to check
     * @return true if OK
     */
    virtual unsigned long length() const = 0;

    /**
     * Gets the color value at index in this palette
     *
     * @param index to look
     * @param color struct to fill
     * @return true if OK
     */
    virtual bool getColorVirtual(unsigned int index, ColorRGBA& color) = 0;

    /**
     * Sets the color value at index in this palette
     *
     * @param index to set
     * @param color struct to use
     * @return true if OK
     */
    virtual bool setColorVirtual(unsigned int index, ColorRGBA& color) = 0;
public:
    /**
     * IPalette destructor
     */
    virtual ~IPalette() = default;

    /**
     * Sets this palette from other palette
     *
     * @return true if OK
     */
    bool set(IPalette& palette);

    /**
     * Checks index of palette is valid
     *
     * @param index to check
     * @return true if OK
     */
    bool checkIndex(unsigned int index);

    /**
     * Gets the color value at index in this palette
     *
     * @param index to look
     * @param color struct to fill
     * @return true if OK
     */
    bool getColor(unsigned int index, ColorRGBA& color);

    /**
     * Sets the color value at index in this palette
     *
     * @param index to set
     * @param color struct to use
     * @return true if OK
     */
    bool setColor(unsigned int index, ColorRGBA& color);
};

#endif //OPENE2140_IPALETTE_H
