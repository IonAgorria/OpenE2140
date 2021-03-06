//
// Created by Ion Agorria on 29/04/18
//
#include "engine/core/utils.h"
#include "palette.h"

Palette::Palette(unsigned int size, bool extra): extra(extra) {
    dirty = false;

    //Reserve colors
    ColorRGBA color;
    for (unsigned int i = 0; i < size; ++i) {
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 0;
        colors.push_back(color);
    }

    if (!Utils::isFlag(FLAG_HEADLESS)) {
        //Create texture
        glActiveTexture(extra ? TEXTURE_UNIT_PALETTE_EXTRA : TEXTURE_UNIT_PALETTE_COLORS);
        glGenTextures(1, &texture);
        error = Utils::checkGLError();
        if (!error.empty()) {
            return;
        }
        bindTexture();

        //Repeat texture when texcoord overflows
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        error = Utils::checkGLError();
        if (!error.empty()) {
            return;
        }

        //Pixel scaling
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        error = Utils::checkGLError();
        if (!error.empty()) {
            return;
        }
    }
}

Palette::~Palette() {
    if (texture) {
        glDeleteTextures(1, &texture);
        //Remove ref
        texture = 0;
    }
}

unsigned long Palette::length() const {
    return colors.size();
}

bool Palette::isExtra() const {
    return extra;
}

bool Palette::setColors(const Palette* palette, unsigned int srcIndex, unsigned int dstIndex, unsigned int length) {
    for (size_t i = 0; i < length; ++i) {
        ColorRGBA color;
        if (!palette->getColor(srcIndex + i, color)) {
            return false;
        }
        if (!setColor(dstIndex + i, color)) {
            return false;
        }
    }
    return true;
}

bool Palette::setColors(const Palette* palette) {
    return setColors(palette, 0, 0, palette->length());
}

bool Palette::getColor(unsigned int index, ColorRGBA& color) const {
    //Check index
    if (index >= length()) {
        return false;
    }
    color.setRGBA(colors[index]);
    return true;
}

bool Palette::setColor(unsigned int index, const ColorRGBA& color) {
    //Check index
    if (index >= length()) {
        error = "Index out of bounds: " + std::to_string(index) + " color " + color.toString();
        return false;
    }
    colors[index].setRGBA(color);
    dirty = true;
    return true;
}

bool Palette::setColor(unsigned int index, const ColorRGB& color) {
    //Check index
    if (index >= length()) {
        error = "Index out of bounds: " + std::to_string(index) + " color " + color.toString();
        return false;
    }
    colors[index].setRGB(color);
    dirty = true;
    return true;
}

Palette::operator bool() {
    return texture != 0;
}

bool Palette::check() {
    if (!error.empty()) {
        return false;
    }
    if (!*this) {
        error = "Palette not ready";
        return false;
    }
    return true;
}

GLuint Palette::bindTexture() const {
    if (texture) {
        glActiveTexture(extra ? TEXTURE_UNIT_PALETTE_EXTRA : TEXTURE_UNIT_PALETTE_COLORS);
        glBindTexture(GL_TEXTURE_1D, texture);
    }
    return texture;
}

GLuint Palette::getTexture() const {
    return texture;
}

bool Palette::updateTexture() {
    if (dirty) {
        dirty = false;
        bindTexture();
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, colors.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, colors.data());
        error = Utils::checkGLError();
        return error.empty();
    }
    return true;
}

std::string Palette::toStringContent() const {
    return " Length: " + std::to_string(length())
         + " Extra: " + std::to_string(extra)
         + " Texture: " + std::to_string(texture)
            ;
}