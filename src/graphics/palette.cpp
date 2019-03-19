//
// Created by Ion Agorria on 29/04/18
//
#include "core/utils.h"
#include "palette.h"

Palette::Palette(unsigned int size, bool extra): extra(extra) {
    //Reserve colors
    colors.reserve(size);

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

bool Palette::getColorVirtual(unsigned int index, ColorRGBA& color) {
    color.setRGBA(colors[index]);
    return true;
}

bool Palette::setColorVirtual(unsigned int index, ColorRGBA& color) {
    colors[index].setRGBA(color);
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
        error = "Image not ready";
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