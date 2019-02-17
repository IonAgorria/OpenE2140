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
#include "core/errorpossible.h"
#include "ipalette.h"

/**
 * Palette implementation which uses array
 */
class Palette : public IPalette {
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

    unsigned long length() const override;

    bool getColorVirtual(unsigned int index, ColorRGBA& color) override;

    bool setColorVirtual(unsigned int index, ColorRGBA& color) override;

    /**
     * Binds the texture for use
     */
    GLuint bindTexture();

    /**
     * @return texture id
     */
    const GLuint getTexture();
};

#endif //OPENE2140_PALETTE_H
