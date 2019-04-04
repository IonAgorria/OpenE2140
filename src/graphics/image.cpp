//
// Created by Ion Agorria on 21/04/18
//
#include <string>
#include "SDL_surface.h"
#include "core/utils.h"
#include "image.h"
#include "palette.h"

Image::Image(const Vector2& size, bool withPalette) : Image(Rectangle(Vector2(), size), withPalette, nullptr) {
}

Image::Image(const Rectangle& rectangle, bool withPalette, std::shared_ptr<Image> owner) :
        owner(owner),
        rectangle(rectangle),
        withPalette(withPalette)
{
    texture = 0;
    if (owner) {
        //Get the root owner if the owner image has an owner
        while (owner->owner) {
            owner = owner->owner;
        }
        //Use the same texture data
        texture = owner->texture;
        textureSize = owner->textureSize;
    } else {
        //Create texture
        glActiveTexture(withPalette ? TEXTURE_UNIT_IMAGE_PALETTE : TEXTURE_UNIT_IMAGE_RGBA);
        glGenTextures(1, &texture);
        error = Utils::checkGLError();
        if (!error.empty()) {
            return;
        }
        bindTexture();

        //Repeat texture when texcoord overflows
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        error = Utils::checkGLError();
        if (!error.empty()) {
            return;
        }

        //Pixel scaling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        error = Utils::checkGLError();
        if (!error.empty()) {
            return;
        }

        //Store the texture size
        textureSize = Vector2(rectangle.w, rectangle.h);

        //Set the initial texture data
        size_t bufferSize = static_cast<const size_t>(textureSize.x * textureSize.y);
        std::unique_ptr<byteArray> buffer = Utils::createBuffer(withPalette ? bufferSize : bufferSize * 4);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            withPalette ? GL_R8UI : GL_RGBA,
            rectangle.w, rectangle.h,
            0,
            withPalette ? GL_RED_INTEGER : GL_RGBA,
            GL_UNSIGNED_BYTE,
            buffer.get()
        );
    }

    updateUVs();
}

Image::~Image() {
    if (texture) {
        if (owner) {
            owner.reset();
        } else {
            //I'm the owner
            glDeleteTextures(1, &texture);
        }
        //Remove ref
        texture = 0;
    }
}

Image::operator bool() {
    return texture != 0;
}

void Image::updateUVs() {
    u = rectangle.x / (float) textureSize.x;
    v = rectangle.y / (float) textureSize.y;
    u2 = (rectangle.x + rectangle.w) / (float) textureSize.x;
    v2 = (rectangle.y + rectangle.h) / (float) textureSize.y;
}

void Image::setRectangle(Rectangle& newPectangle) {
    this->rectangle = Rectangle(newPectangle);
    updateUVs();
}

const Rectangle& Image::getRectangle() const {
    return rectangle;
}

bool Image::check(bool usePalette) {
    if (!error.empty()) {
        return false;
    }
    if (!*this) {
        error = "Image not ready";
        return false;
    }
    if (usePalette != withPalette) {
        error = "Palette type mismatch";
        return false;
    }
    return true;
}

const GLuint Image::getTexture() {
    return texture;
}

void Image::setPalette(std::shared_ptr<Palette> newPalette) {
    this->palette = newPalette;
}

std::shared_ptr<Palette> Image::getPalette() {
    return palette;
}

GLuint Image::bindTexture() {
    if (texture) {
        glActiveTexture(this->withPalette ? TEXTURE_UNIT_IMAGE_PALETTE : TEXTURE_UNIT_IMAGE_RGBA);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    return texture;
}

bool Image::loadTextureR8(const byte* pixels) {
    bindTexture();
    std::unique_ptr<byteArray> flipped = Utils::bufferFlipY(pixels, rectangle.w, rectangle.h);
    glTexSubImage2D(GL_TEXTURE_2D, 0, rectangle.x, rectangle.y, rectangle.w, rectangle.h, GL_RED_INTEGER, GL_UNSIGNED_BYTE, flipped.get());
    error = Utils::checkGLError();
    return error.empty();
}

bool Image::loadTextureRGBA(const byte* pixels) {
    bindTexture();
    std::unique_ptr<byteArray> flipped = Utils::bufferFlipY(pixels, rectangle.w * 4, rectangle.h);
    glTexSubImage2D(GL_TEXTURE_2D, 0, rectangle.x, rectangle.y, rectangle.w, rectangle.h, GL_RGBA, GL_UNSIGNED_BYTE, flipped.get());
    error = Utils::checkGLError();
    return error.empty();
}

bool Image::loadFromIndexed8(const byte* pixels) {
    if (!check(true)) return false;

    //Load data to texture
    return loadTextureR8(pixels);
}

bool Image::loadFromRGB565(const byte* pixels) {
    if (!check(false)) return false;

    //Create buffer for converted pixels and do conversion
    std::unique_ptr<byteArray> converted = Utils::createBuffer(
            static_cast<const size_t>(rectangle.w * rectangle.h * 4)
    );
    int result = SDL_ConvertPixels(
            rectangle.w, rectangle.h,
            SDL_PIXELFORMAT_RGB565, pixels, rectangle.w * 2,
            SDL_PIXELFORMAT_ABGR8888, converted.get(), rectangle.w * 4
    );
    if (result < 0) {
        error = "Error converting RGB565 " + Utils::checkSDLError();
        return false;
    }

    //Load converted data and return result
    return loadTextureRGBA(converted.get());
}

bool Image::loadFromRGBA8888(const byte* pixels) {
    if (!check(false)) return false;

    //Load data to texture
    return loadTextureRGBA(pixels);
}

std::string Image::toString() const {
    return "Image(" + toStringContent() + ")";
}

std::string Image::toStringContent() const {
    return " Rectangle: " + rectangle.toString()
         + " With palette: " + std::to_string(withPalette)
         + " Palette: " + (palette ? palette->toString() : "null")
            ;
}
