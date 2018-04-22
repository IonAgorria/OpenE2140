//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_IMAGE_H
#define OPENE2140_IMAGE_H

#include "core/errorpossible.h"
#include "core/math/rectangle.h"
#include "core/config.h"
#include "texture.h"

/**
 * Image instance used for window drawing in abstract way
 */
class Image {
private:
    /**
     * Texture containing this image data
     */
    std::shared_ptr<Texture> texture;

    /**
     * Rectangle to know texture source
     */
     Rectangle rectangle;
public:
    /**
     * Constructor for image
     */
    Image(std::shared_ptr<Texture> texture, Rectangle& rectangle);

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
    operator WindowTexture();

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
     * @param log to use
     * @param pixels to fill the rectangle
     * @return if success
     */
    bool loadFromRGB565(const log_ptr log, const byte* pixels);

    /**
     * Loads image data to texture using pixels in RGB8888 format.
     * Pixels array must match rectangle of image.
     *
     * @param log to use
     * @param pixels to fill the rectangle
     * @return if success
     */
    bool loadFromRGBA8888(const log_ptr log, const byte* pixels);
};

#endif //OPENE2140_IMAGE_H
