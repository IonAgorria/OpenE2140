//
// Created by Ion Agorria on 22/04/18
//
#ifndef OPENE2140_TEXTURE_H
#define OPENE2140_TEXTURE_H

#include <SDL_render.h>
#include "core/config.h"

using WindowTexture = SDL_Texture*;

/**
 * Contains texture inside the GPU for drawing later in the window
 */
class Texture {
    /** Texture width */
    const int width;
    /** Texture height */
    const int height;
    /** Texture itself */
    WindowTexture texture;
public:
    /**
     * Constructor for texture
     *
     * @param texture to manage
     * @param width of texture
     * @param height of texture;
     */
    Texture(WindowTexture texture, const int width, const int height);

    /**
     * Texture destructor
     */
    ~Texture();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Texture)

    /**
     * @return if texture is valid
     */
    operator bool();

    /**
     * @return texture itself
     */
    operator WindowTexture();
};

#endif //OPENE2140_TEXTURE_H
