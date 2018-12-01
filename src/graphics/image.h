//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_IMAGE_H
#define OPENE2140_IMAGE_H

#include <memory>
#include <SDL_opengl.h>
#include "core/common.h"
#include "core/errorpossible.h"
#include "math/rectangle.h"
#include "palette.h"

/**
 * Image instance used for window drawing in abstract way
 */
class Image: public IErrorPossible {
private:
    /**
     * Texture reference containing this image data
     */
    GLuint texture = 0;

    /**
     * Pointer to the image which the texture belongs
     */
    std::shared_ptr<Image> owner;

    /**
     * Color palette for this image if any
     */
    std::shared_ptr<Palette> paletteColors;

    /**
     * Extra palette for this image if any
     */
    std::shared_ptr<Palette> paletteExtra;

    /**
     * Rectangle to know texture source
     */
    Rectangle rectangle;

    /**
     * Checks if image is correct
     *
     * @return true if OK
     */
    bool check();

    /**
     * Loads image data to texture in this image.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle allocated to this image into texture
     * @param bytes bytes per pixel
     * @return if success
     */
    bool loadTexture(const byte* pixels, int bytes);
public:
    /**
     * Constructor for a image containing the entire texture
     */
    Image(const Vector2& size);

    /**
     * Constructor for image using a subset of an texture
     */
    Image(std::shared_ptr<Image> owner, const Rectangle& rectangle);

    /**
     * Image destructor
     */
    ~Image();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Image)

    /**
     * @return if image is valid
     */
    operator bool();

    /**
     * @return rectangle of this image inside the texture
     */
    Rectangle& getRectangle();

    /**
     * @return rectangle of this image inside the texture
     */
    const Rectangle& getRectangle() const;

    /**
     * Binds the texture for use
     */
    bool bindTexture();

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
