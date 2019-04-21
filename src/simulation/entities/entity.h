//
// Created by Ion Agorria on 2018/06/03
//
#ifndef OPENE2140_ENTITY_H
#define OPENE2140_ENTITY_H

#include <vector>
#include "core/common.h"
#include "math/rectangle.h"
#include "graphics/image.h"

/**
 * Base entity in game
 */
class Simulation;
class Entity {
private:
    /**
     * Unique ID for entity
     */
    long id;

    /**
     * Simulation reference which this entity belongs
     */
    const Simulation& simulation;

    /**
     * Images used by this entity
     */
    const std::vector<std::shared_ptr<Image>> images;

public:
    /**
     * Entity position and size;
     */
    Rectangle rectangle;

    /**
     * Entity constructor
     */
    Entity();

    /**
     * Entity destructor
     */
    virtual ~Entity() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Entity)

    /**
     * Sets the entity simulation when entity is added or loaded from saved state
     */
    virtual bool setupSimulation(const Simulation& simulation);

    /**
     * @return string version of this entity
     */
    virtual std::string toString() const;

    /**
     * @return string content of this entity
     */
    virtual std::string toStringContent() const;
};

#endif //OPENE2140_ENTITY_H
