//
// Created by Ion Agorria on 2018/06/03
//
#ifndef OPENE2140_ENTITY_H
#define OPENE2140_ENTITY_H

#include "components/component.h"
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
     * Entity center position
     */
    Vector2 position;

    /**
     * Entity bounding rectangle if any
     */
    Rectangle bounds;

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
     * @return entity bounds
     */
    const Rectangle& getBounds();

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
     *
     * @param renderer to use for drawing
     */
    void draw(Renderer* renderer);

    /**
     * @return true if entity is considered active (has ID and is inside simulation)
     */
    bool active();

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
 */
#define CLASS_ENTITY(T_ENTITY, ...) \
class T_ENTITY: public ComponentBinder<Entity, ##__VA_ARGS__> { \
public: \
    /** Update type name */ \
    TYPE_NAME_OVERRIDE(T_ENTITY)

#endif //OPENE2140_ENTITY_H
