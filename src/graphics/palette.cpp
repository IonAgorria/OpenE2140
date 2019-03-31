//
// Created by Ion Agorria on 29/04/18
//
#include "core/utils.h"
#include "palette.h"

Palette::Palette(unsigned int size, bool extra): extra(extra) {
    //Reserve colors
    ColorRGBA color;
    for (int i = 0; i < size; ++i) {
        colors.push_back(color);
    }

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

bool Palette::set(Palette& palette) {
    for (unsigned int i = 0; i < palette.length(); ++i) {
        ColorRGBA color;
        if (!palette.getColor(i, color)) {
            return false;
        }
        if (!setColor(i,color)) {
            return false;
        }
    }
    return true;
}

bool Palette::getColor(unsigned int index, ColorRGBA& color) {
    //Check index
    if (index >= length()) {
        error = "Index out of bounds: " + std::to_string(index);
        return false;
    }
    color.setRGBA(colors[index]);
    return true;
}

bool Palette::setColor(unsigned int index, ColorRGBA& color) {
    //Check index
    if (index >= length()) {
        error = "Index out of bounds: " + std::to_string(index) + " color " + color.toString();
        return false;
    }
    colors[index].setRGBA(color);
    return true;
}

bool Palette::setColor(unsigned int index, ColorRGB& color) {
    //Check index
    if (index >= length()) {
        error = "Index out of bounds: " + std::to_string(index) + " color " + color.toString();
        return false;
    }
    colors[index].setRGB(color);
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

GLuint Palette::bindTexture() {
    if (texture) {
        glActiveTexture(extra ? TEXTURE_UNIT_PALETTE_EXTRA : TEXTURE_UNIT_PALETTE_COLORS);
        glBindTexture(GL_TEXTURE_1D, texture);
    }
    return texture;
}

const GLuint Palette::getTexture() {
    return texture;
}

bool Palette::updateTexture() {
    bindTexture();
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, colors.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, colors.data());
    error = Utils::checkGLError();
    return error.empty();
}