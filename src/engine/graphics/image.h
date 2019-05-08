//
// Created by Ion Agorria on 21/04/18
//
#ifndef OPENE2140_IMAGE_H
#define OPENE2140_IMAGE_H

#include <memory>
#include "core/common.h"
#include "core/error_possible.h"
#include "math/rectangle.h"

/**
 * Image instance used for window drawing in abstract way
 */
class Palette;
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
     * Flag to specify if this image will use a palette
     */
    bool withPalette;

    /**
     * Checks if image is correct
     *
     * @param usePalette is current call for a palette type?
     * @return true if OK
     */
    bool check(bool usePalette);

    /**
     * Loads single channel 8 bit image data to texture in this image.
     *
     * @param pixels to fill the rectangle allocated to this image into texture
     * @return if success
     */
    bool loadTextureR8(const byte* pixels);

    /**
     * Loads RGBA image data to texture in this image.
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
     * Image texture U2
     */
    float u2;
    /**
     * Image texture V
     */
    float v;
    /**
     * Image texture V2
     */
    float v2;

    /**
     * Constructor for a image containing the entire texture
     */
    Image(const Vector2& size, bool withPalette);

    /**
     * Constructor for image using a subset of an texture
     */
    Image(const Rectangle& rectangle, bool withPalette, std::shared_ptr<Image> owner);

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
    void setRectangle(Rectangle& newRectangle);

    /**
     * @return rectangle of this image inside the texture
     */
    const Rectangle& getRectangle() const;

    /**
     * @return texture id
     */
    const GLuint getTexture() const;

    /**
     * Set the palette to use with this image
     */
    void setPalette(std::shared_ptr<Palette> newPalette);

    /**
     * @return palette used by this image if any
     */
    const std::shared_ptr<Palette> getPalette() const;

    /**
     * Binds the texture for use
     */
    GLuint bindTexture() const;

    /**
     * Loads image data to texture using pixels in indexed format.
     * 8bits each pixel (color index 8 bits)
     * Pixels array must match rectangle of image.
     *
     * @param pixels indices to fill the rectangle
     * @return if success
     */
    bool loadFromIndexed8(const byte* pixels);

    /**
     * Loads image data to texture using pixels in RGB565 format.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle
     * @param alpha data for each pixel
     * @return if success
     */
    bool loadFromRGB565(const byte* pixels);

    /**
     * Loads image data to texture using pixels in RGBA8888 format.
     * Pixels array must match rectangle of image.
     *
     * @param pixels to fill the rectangle
     * @return if success
     */
    bool loadFromRGBA8888(const byte* pixels);

    /**
     * @return string version of this
     */
    std::string toString() const;

    /**
    * @return string content of this
    */
    std::string toStringContent() const;
};

#endif //OPENE2140_IMAGE_H
