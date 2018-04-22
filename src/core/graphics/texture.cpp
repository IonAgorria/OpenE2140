//
// Created by Ion Agorria on 22/04/18
//
#include "texture.h"

Texture::Texture(WindowTexture texture, const int width, const int height) : texture(texture), width(width), height(height) {
}

Texture::~Texture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

Texture::operator bool() {
    return texture != nullptr;
}

Texture::operator WindowTexture() {
    return texture;
}
