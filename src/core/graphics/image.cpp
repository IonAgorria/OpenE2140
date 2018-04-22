//
// Created by Ion Agorria on 21/04/18
//
#include <string>
#include "SDL_surface.h"
#include "core/utils.h"
#include "image.h"

Image::Image(texture_ptr texture, Rectangle& rectangle) : texture(texture), rectangle(rectangle) {
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

bool Image::loadFromRGB565(const log_ptr log, const byte* pixels) {
    if (!texture) {
        log->error("Error loading image, texture not available");
        return false;
    }
    //Create buffer for converted pixels
    std::unique_ptr<byteArray> converted = Utils::createBuffer(
            static_cast<const size_t>(rectangle.w * rectangle.h * 4)
    );
    int result = SDL_ConvertPixels(
            rectangle.w, rectangle.h,
            SDL_PIXELFORMAT_RGB565, pixels, rectangle.w * 2,
            SDL_PIXELFORMAT_RGBA8888, converted.get(), rectangle.w * 4
    );
    if (result < 0) {
        log->error("Error converting RGB565 {0}", Utils::checkSDLError());
        return false;
    }
    return loadFromRGBA8888(std::move(log), converted.get());
}

bool Image::loadFromRGBA8888(const log_ptr log, const byte* pixels) {
    if (SDL_UpdateTexture(texture.get(), NULL, pixels, rectangle.w * 4) != 0) {
        log->error("Couldn't update texture {0}", Utils::checkSDLError());
        return false;
    }
    return true;
}
