//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_IMAGE_GROUP_COMPONENT_H
#define OPENE2140_IMAGE_GROUP_COMPONENT_H

#include "engine/graphics/renderer.h"
#include "engine/graphics/palette.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/entities/entity.h"
#include "engine/simulation/components/image_component.h"

class Image;

/**
 * Contains several images grouped
 */
CLASS_COMPONENT(Entity, ImageGroupComponent)
protected:
    /**
     * Image to draw
     */
    Image* image = nullptr;

    /**
     * Image offset from entity center
     */
    Vector2 imageOffset;

    /**
     * Image size
     */
    Vector2 imageSize;

    /**
     * Image direction
     */
    float imageDirection = 0;

    /**
     * Flag for extra palette
     */
    bool hasExtraPalette = false;

    /**
     * Optional palette to provide when drawing image
     */
    std::unique_ptr<Palette> extraPalette;

public:

    /**
     * Draws this image
     *
     * @param renderer
     */
    void draw(Renderer* renderer);
};

#endif //OPENE2140_IMAGE_GROUP_COMPONENT_H
