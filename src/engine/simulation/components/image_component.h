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
 * Adds a single image
 */
CLASS_COMPONENT(Entity, ImageComponent)
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

public:
    /**
     * Draws this image
     *
     * @param renderer
     */
    void draw(Renderer* renderer);

    /**
     * Sets the image from sprite
     *
     * @param code sprite to set
     */
    void setImageFromSprite(const std::string& code);
};

#endif //OPENE2140_IMAGE_COMPONENT_H
