//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_IMAGE_GROUPS_COMPONENT_H
#define OPENE2140_IMAGE_GROUPS_COMPONENT_H

#include "engine/graphics/renderer.h"
#include "engine/graphics/palette.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/entity.h"
#include "engine/simulation/components/component.h"

class Image;

/**
 * Contains several images grouped
 */
class ImageGroupsComponent {
CLASS_COMPONENT_BODY(Entity, ImageGroupsComponent)
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

    /**
     * Optional palette to provide when drawing image
     */
    std::unique_ptr<Palette> extraPalette;

public:
    /**
     * Destructor
     */
    ~ImageGroupsComponent() = default;

    void construction() {
        if (hasExtraPalette) {
            extraPalette = std::make_unique<Palette>();
        }
    }

    void simulationChanged() {}

    void update() {}

    /**
     * Draws this image
     *
     * @param renderer
     */
    void draw(Renderer* renderer) {
        if (image) {
            Vector2 position = base->getPosition();
            position += imageOffset;
            if (hasExtraPalette) {
                renderer->draw(position, imageSize, imageDirection, *image, extraPalette.get());
            } else {
                renderer->draw(position, imageSize, imageDirection, *image, nullptr);
            }
        }
    }
};

#endif //OPENE2140_IMAGE_GROUPS_COMPONENT_H
