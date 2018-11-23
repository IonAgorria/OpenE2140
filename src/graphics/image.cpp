//
// Created by Ion Agorria on 21/04/18
//
#include <string>
#include "SDL_surface.h"
#include "core/utils.h"
#include "image.h"

Image::Image(const Vector2& size) : Image(nullptr, Rectangle(Vector2(), size)) {
}

Image::Image(std::shared_ptr<Image> owner, const Rectangle& rectangle) :
        owner(owner),
        rectangle(rectangle)
{
    if (owner) {
        //Use the same texture
        texture = owner->texture;
        paletteColors = owner->paletteColors;
        paletteExtra = owner->paletteExtra;
    } else {
        //Create texture
        glActiveTexture(TEXTURE_UNIT_IMAGE);
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
    }
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

Rectangle& Image::getRectangle() {
    return rectangle;
}

const Rectangle& Image::getRectangle() const {
    return rectangle;
}

bool Image::check() {
    if (!error.empty()) {
        return false;
    }
    if (!*this) {
        error = "Image not ready";
        return false;
    }
    return true;
}

bool Image::bindTexture() {
    if (texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
        return true;
    }
    return false;
}

bool Image::loadTexture(const byte* pixels, int bytes) {
    /*
    if (SDL_UpdateTexture(texture.get(), &rectangle, pixels, rectangle.w * bytes) != 0) {
        error = "Couldn't update texture " + Utils::checkSDLError();
        return false;
    }*/
    return true;
}

bool Image::loadFromRGB565(const byte* pixels, const byte* alpha) {
    if (!check()) return false;

    //Create buffer for converted pixels and do conversion
    std::unique_ptr<byteArray> converted = Utils::createBuffer(
            static_cast<const size_t>(rectangle.w * rectangle.h * 4)
    );
    int result = SDL_ConvertPixels(
            rectangle.w, rectangle.h,
            SDL_PIXELFORMAT_RGB565, pixels, rectangle.w * 2,
            SDL_PIXELFORMAT_RGBA8888, converted.get(), rectangle.w * 4
    );
    if (result < 0) {
        error = "Error converting RGB565 " + Utils::checkSDLError();
        return false;
    }

    //Update the alpha
    size_t size = static_cast<const size_t>(rectangle.w * rectangle.h);
    for (size_t i = 0; i < size; i++) {
        converted[i * 4] = alpha == nullptr ? (byte) 0xFF : alpha[i];
    }

    //Load converted data and return result
    return loadTexture(converted.get(), 4);
}

bool Image::loadFromRGBA8888(const byte* pixels) {
    if (!check()) return false;

    //Load data to texture
    return loadTexture(pixels, 4);
}
