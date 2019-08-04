//
// Created by Ion Agorria on 2018/06/03
//
#ifndef OPENE2140_ENTITY_H
#define OPENE2140_ENTITY_H

#include "engine/simulation/components/component.h"
#include "engine/core/common.h"
#include "engine/core/to_string.h"
#include "engine/math/rectangle.h"
#include "engine/graphics/image.h"

class Tile;
class Simulation;
class Renderer;
class EntityConfig;

/**
 * Base entity in game, this is the common interface between world and entities
 */
class Entity: public IToString {
protected:
    /**
     * Unique ID for entity
     */
    entity_id_t id = 0;

    /**
     * This entity config such as type and stats
     */
    const EntityConfig* config;

    /**
     * Simulation which this entity belongs
     */
    Simulation* simulation;

    /**
     * Renderer to use when drawing this entity
     */
    Renderer* renderer;

    /**
     * Entity center position
     */
    Vector2 position;

    /**
     * Entity direction which is facing
     */
    entity_direction_t direction;

    /**
     * Entity bounding rectangle if any
     */
    Rectangle bounds;

    /**
     * Tile or tiles which this entity is currently occupying
     */
    std::vector<Tile*> tiles;

    /**
     * Flag for active state
     */
    bool active;

    /**
     * Flag for disable state
     */
    bool disable;

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
     * Does the entity setup after object creation
     */
    void setup(EntityConfig* newConfig);

    /**
     * @return entity position
     */
    const Vector2& getPosition() const;

    /**
     * Set entity position
     *
     * @param newPosition position to set
     */
    void setPosition(const Vector2& newPosition);

    /**
     * @return entity direction
     */
    entity_direction_t getDirection() const;

    /**
     * Set entity direction
     *
     * @param newDirection direction to set
     */
    void setDirection(entity_direction_t newDirection);

    /**
     * @return entity bounds
     */
    const Rectangle& getBounds() const;

    /**
     * Set entity bounds
     *
     * @param newBounds bounds to set
     */
    void setBounds(const Rectangle& newBounds);

    /**
     * @return entity config if any
     */
    const EntityConfig* getConfig() const;

    /**
     * @return entity simulation if any
     */
    Simulation* getSimulation() const;

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
     * Called when this entity is requested to draw
     */
    virtual void draw();

    /**
     * @return true if entity is considered active (has ID and is inside simulation)
     */
    bool isActive();

    /**
     * @return true if entity is considered disabled
     */
    bool isDisable() const;

    /**
     * Sets this entity disable state
     *
     * @param newDisable state to set
     */
    void setDisable(bool newDisable);

    /**
     * In case of none this method will return null
     * In case of several tiles it will return the first tile in collection, which should be the center
     * @return tile which this entity is occupying
     */
    Tile* getTile();

    /**
     * @return tile or tiles which this entity is occupying
     */
    const std::vector<Tile*>& getTiles();

    /*
     * IToString
     */

    std::string toString() const override;

    std::string toStringContent() const override;
};

/**
 * Macro for common entity class definition with provided components
 *
 * T_ENTITY specifies the class type name
 * The rest of args are the component classes to be attached into this class
 */
#define CLASS_ENTITY(T_BASE, T_ENTITY, ...) \
class T_ENTITY: public ComponentBinder<T_BASE, T_ENTITY ,##__VA_ARGS__> { \
public: \
    /** Update type name */ \
    TYPE_NAME_OVERRIDE(T_ENTITY)

#endif //OPENE2140_ENTITY_H
