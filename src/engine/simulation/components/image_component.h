//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_IMAGE_COMPONENT_H
#define OPENE2140_IMAGE_COMPONENT_H

#include "engine/graphics/renderer.h"
#include "engine/graphics/animation.h"
#include "engine/graphics/palette.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/components/image_component.h"

class Entity;
class Image;

/**
 * Contains animation and extra palette drawing
 */
CLASS_COMPONENT(Entity, ImageComponent)
protected:
    /**
     * Image to draw from animation
     */
    Image* image = nullptr;

    /**
     * Animation to draw in component
     */
    std::unique_ptr<Animation> animation;

    /**
     * Optional palette to provide when drawing image
     */
    std::unique_ptr<Palette> extraPalette;

public:
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
     * Animation play state
     */
    bool animationPlay = true;

    /**
     * Extra palette size if any
     */
    unsigned int extraPaletteSize = 0;

    /**
     * Draws this component
     *
     * @param renderer
     */
    void draw(Renderer* renderer);

    /**
     * @return animation in component
     */
    Animation* getAnimation() const;

    /**
     * @return extra palette in component if any
     */
    Palette* getExtraPalette() const;

    /**
     * Sets the image from sprite
     *
     * @param code sprite to set
     */
    void setImageFromSprite(const std::string& code);

    /**
     * Sets the animation from sprite
     *
     * @param code sprite to set
     * @param restart resets the animation to 0
     */
    void setAnimationFromSprite(const std::string& code, bool restart = true);
};

#endif //OPENE2140_IMAGE_COMPONENT_H
