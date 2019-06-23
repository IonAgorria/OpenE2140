//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_IMAGE_COMPONENT_H
#define OPENE2140_IMAGE_COMPONENT_H

#include "engine/graphics/renderer.h"
#include "engine/graphics/palette.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/entities/entity.h"
#include "engine/simulation/components/component.h"

class Image;

/**
 * Adds a single image and optional extra palette
 */
CLASS_COMPONENT(Entity, ImageComponent)
protected:
    /**
     * Image to draw
     */
    std::shared_ptr<Image> image;

    /**
     * Image offset from entity center;
     */
    Vector2 imageOffset;

    /**
     * Image size;
     */
    Vector2 imageSize;

    /**
     * Image direction;
     */
    float imageDirection;

public:
    /**
     * Draws this image
     *
     * @param renderer
     */
    void draw(Renderer* renderer);
};

#endif //OPENE2140_IMAGE_COMPONENT_H
