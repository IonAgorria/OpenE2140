//
// Created by Ion Agorria on 21/04/18
//
#include <string>
#include "SDL_surface.h"
#include "core/utils.h"
#include "image.h"

Image::Image(texture_ptr texture, const Rectangle& rectangle) : texture(texture), rectangle(rectangle) {
}

Image::Image(texture_ptr texture, const Vector2& size) : texture(texture), rectangle(Rectangle(Vector2(), size)) {
}

Image::operator bool() {
    return texture != nullptr;
}

Image::operator texture_ptr() {
    return texture;
}

Rectangle& Image::getRectangle() {
    return rectangle;
}

const Rectangle& Image::getRectangle() const {
    return rectangle;
}

bool Image::check() {
    if (!texture) {
        error = "Error loading image, texture not available";
        return false;
    }
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
    if (!loadAlpha(converted.get(), alpha)) {
        return false;
    }

    //Load converted data and return result
    return loadFromRGBA8888(converted.get());
}

bool Image::loadFromRGB888(const byte* pixels, const byte* alpha) {
    if (!check()) return false;

    //Create buffer for converted pixels and do conversion
    std::unique_ptr<byteArray> converted = Utils::createBuffer(
            static_cast<const size_t>(rectangle.w * rectangle.h * 4)
    );
    int result = SDL_ConvertPixels(
            rectangle.w, rectangle.h,
            SDL_PIXELFORMAT_RGB888, pixels, rectangle.w * 3,
            SDL_PIXELFORMAT_RGBA8888, converted.get(), rectangle.w * 4
    );
    if (result < 0) {
        error = "Error converting RGB888 " + Utils::checkSDLError();
        return false;
    }

    //Update the alpha
    if (!loadAlpha(converted.get(), alpha)) {
        return false;
    }

    //Load converted data and return result
    return loadFromRGBA8888(converted.get());
}

bool Image::loadAlpha(byte* pixels, const byte* alpha) {
    size_t size = static_cast<const size_t>(rectangle.w * rectangle.h);
    for (size_t i = 0; i < size; i++) {
        pixels[i * 4] = alpha == nullptr ? (byte) 0xFF : alpha[i];
    }
    return true;
}

bool Image::loadFromRGBA8888(const byte* pixels) {
    if (!check()) return false;

    //Load data to texture
    if (SDL_UpdateTexture(texture.get(), NULL, pixels, rectangle.w * 4) != 0) {
        error = "Couldn't update texture " + Utils::checkSDLError();
        return false;
    }
    return true;
}
