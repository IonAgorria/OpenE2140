//
// Created by Ion Agorria on 2018/06/03
//
#ifndef OPENE2140_ENTITY_H
#define OPENE2140_ENTITY_H

#include "core/common.h"
#include "math/rectangle.h"
#include "graphics/image.h"

class Simulation;

/**
 * Base entity in game
 */
class Entity {
protected:
    /**
     * Unique ID for entity
     */
    long id = 0;

    /**
     * Simulation which this entity belongs
     */
    Simulation* simulation;

    /**
     * Entity position
     */
    Vector2 position;

    /**
     * Images used by this entity
     */
    std::vector<std::shared_ptr<Image>> images;

public:
    TYPE_NAME(Entity)

    /**
     * Entity constructor
     */
    Entity();

    /**
     * Entity destructor
     */
    virtual ~Entity();

    /**
     * Disable copy
     */
    NON_COPYABLE(Entity)

    /**
     * @return entity position
     */
    const Vector2& getPosition();

    /**
     * Called when entity is added to simulation
     */
    virtual void addedToSimulation(Simulation* simulation);

    /**
     * Called when entity is added to simulation
     */
    virtual void removedFromSimulation();

    /**
     * @return string version of this entity
     */
    virtual std::string toString() const;

    /**
     * @return string content of this entity
     */
    virtual std::string toStringContent() const;

    /**
     * Called when this entity is going to be drawn
     *
     * @param drawPosition center to draw the image
     * @param drawSize size of rectangle to draw
     * @param drawAngle angle of image when drawing
     * @param palette extra palette for this entity to draw custom colors
     * @return the image to be used when drawing this entity
     */
    virtual Image* draw(Vector2& drawPosition, Vector2& drawSize, float& drawAngle, Palette* palette);
};

#endif //OPENE2140_ENTITY_H
