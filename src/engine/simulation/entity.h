//
// Created by Ion Agorria on 2018/06/03
//
#ifndef OPENE2140_ENTITY_H
#define OPENE2140_ENTITY_H

#include "component.h"
#include "core/common.h"
#include "core/to_string.h"
#include "math/rectangle.h"
#include "graphics/image.h"

class Tile;
class Simulation;

/**
 * Base entity in game, this is the common interface between world and entities
 */
class Entity: public IToString {
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
     * Tile or tiles which this entity is currently occupying
     */
    std::vector<Tile*> tiles;

    /**
     * Images used by this entity
     */
    std::vector<std::shared_ptr<Image>> images;

    /**
     * Add components method forwarding so extended entities can override them
     */
    COMPONENT_METHODS(COMPONENT_METHOD_FORWARD_VIRTUAL)
public:
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
     * Type name
     */
    virtual TYPE_NAME(Entity)

    /**
     * @return entity position
     */
    const Vector2& getPosition();

    /**
     * Called when entity is added to simulation
     */
    void addedToSimulation(Simulation* simulation);

    /**
     * Called when entity is added to simulation
     */
    void removedFromSimulation();

    /**
     * Updates the entity state
     */
    void update();

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

    std::string toString() const override;

    std::string toStringContent() const override;
};

/**
 * Macro for common entity class definition with provided components
 */
#define CLASS_ENTITY(T_ENTITY, ...) \
class T_ENTITY: public ComponentBinder<Entity, ##__VA_ARGS__> { \
public: \
    /** Update type name */ \
    TYPE_NAME_OVERRIDE(T_ENTITY)

#endif //OPENE2140_ENTITY_H
