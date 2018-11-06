//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_IMAGE_H
#define OPENE2140_IMAGE_H

#include <memory>
#include "core/config.h"
#include "core/errorpossible.h"
#include "math/rectangle.h"
#include "SDL_render.h"

/**
 * Texture used for storing image on GPU
 */
using texture_ptr = std::shared_ptr<SDL_Texture>;

/**
 * Image instance used for window drawing in abstract way
 */
class Image: public ErrorPossible {
private:
    /**
     * Texture containing this image data
     */
    const texture_ptr texture;

    /**
     * Pixel format data used in the target texture
     */
    unsigned int textureFormat;

    /**
     * Rectangle to know texture source
     */
    Rectangle rectangle;

    /**
     * Checks if image is correct
     *
     * @param format required for texture
     * @return true if OK
     */
    bool check(unsigned int format);

    /**
     * Loads alpha data into pixels array
     *
     * @param pixels to update
     * @param alpha to set
     * @return true if OK
     */
    bool loadAlpha(byte* pixels, const byte* alpha);

    /**
     * Loads image data to texture.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle allocated to this image into texture
     * @param bytes bytes per pixel
     * @return if success
     */
    bool loadTexture(const byte* pixels, int bytes);
public:
    /**
     * Constructor for image
     */
    Image(texture_ptr texture, const Rectangle& rectangle);

    /**
     * Constructor for image
     */
    Image(texture_ptr texture, const Vector2& size);

    /**
     * Image destructor
     */
    ~Image() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Image)

    /**
     * @return if image was created
     */
    operator bool();

    /**
     * @return image window texture
     */
    operator texture_ptr();

    /**
     * @return rectangle of this image inside the texture
     */
    Rectangle& getRectangle();

    /**
     * @return rectangle of this image inside the texture
     */
    const Rectangle& getRectangle() const;

    /**
     * Loads image data to texture using pixels in RGB565 format.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle
     * @param alpha data for each pixel
     * @return if success
     */
    bool loadFromRGB565(const byte* pixels, const byte* alpha);

    /**
     * Loads image data to texture using pixels in RGBA8888 format.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle
     * @return if success
     */
    bool loadFromRGBA8888(const byte* pixels);
};

#endif //OPENE2140_IMAGE_H
