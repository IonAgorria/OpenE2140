//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_IMAGE_H
#define OPENE2140_IMAGE_H

#include <memory>
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
    std::shared_ptr<Palette> palette;

    /**
     * Texture size
     */
    Vector2 textureSize;

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
     * Loads single channel image data to texture in this image.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle allocated to this image into texture
     * @return if success
     */
    bool loadTextureR(const byte* pixels);

    /**
     * Loads RGBA image data to texture in this image.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle allocated to this image into texture
     * @return if success
     */
    bool loadTextureRGBA(const byte* pixels);

    /**
     * Updates UVs for this image based on rectangle and texture size
     */
     void updateUVs();
public:
    /**
     * Image texture U
     */
    float u;
    /**
     * Image texture V
     */
    float u2;
    /**
     * Image texture U2
     */
    float v;
    /**
     * Image texture V2
     */
    float v2;

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
     * Set the rectangle of this image inside the texture
     */
    void setRectangle(Rectangle& rectangle);

    /**
     * @return rectangle of this image inside the texture
     */
    const Rectangle& getRectangle() const;

    /**
     * @return texture id
     */
    const GLuint getTexture();

    /**
     * @return palette used by this image if any
     */
    std::shared_ptr<Palette> getPalette();

    /**
     * Binds the texture for use
     */
    GLuint bindTexture();

    /**
     * Loads image data to texture using pixels in indexed format.
     * Pixels array must match rectangle of image.
     *
     * @param pixels indices to fill the rectangle
     * @return if success
     */
    bool loadFromI8(const byte* pixels);

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
