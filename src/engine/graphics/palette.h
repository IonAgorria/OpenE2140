//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_PALETTE_H
#define OPENE2140_PALETTE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "core/common.h"
#include "core/error_possible.h"
#include "color.h"

/**
 * Palette implementation which uses array for CPU side and 1D texture for GPU side
 */
class Palette : public IErrorPossible {
protected:
    /**
     * Internal palette colors
     */
    std::vector<ColorRGBA> colors;

    /**
     * Texture reference containing this palette data
     */
    GLuint texture = 0;

    /**
     * Checks if image is correct
     *
     * @return true if OK
     */
    bool check();

public:
    /**
     * Is palette for extra colors?
     */
    const bool extra;

    /**
     * Constructor for empty palette
     */
    Palette(unsigned int size, bool extra);

    /**
     * Palette destructor
     */
    ~Palette();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Palette)

    /**
     * @return if palette is valid
     */
    operator bool();

    /**
     * @return palette number of colors
     */
    unsigned long length() const;

    /**
     * Sets this palette from other palette
     *
     * @return true if OK
     */
    bool set(Palette& palette);

    /**
     * Gets the color value at index in this palette
     *
     * @param index to look
     * @param color struct to fill
     * @return true if OK
     */
    bool getColor(unsigned int index, ColorRGBA& color) const;

    /**
     * Sets the color value at index in this palette
     *
     * @param index to set
     * @param color struct to use
     * @return true if OK
     */
    bool setColor(unsigned int index, ColorRGBA& color);

    /**
     * Sets the color value at index in this palette
     *
     * @param index to set
     * @param color struct to use
     * @return true if OK
     */
    bool setColor(unsigned int index, ColorRGB& color);

    /**
     * Binds the texture for use
     */
    GLuint bindTexture() const;

    /**
     * Updates the palette content to texture
     *
     * @return true if OK
     */
    bool updateTexture();

    /**
     * @return texture id
     */
    const GLuint getTexture() const;

    /**
     * @return string version of this
     */
    std::string toString() const;

    /**
    * @return string content of this
    */
    std::string toStringContent() const;
};

#endif //OPENE2140_PALETTE_H
