//
// Created by Ion Agorria on 2018/06/03
//
#ifndef OPENE2140_ENTITY_H
#define OPENE2140_ENTITY_H

#include <vector>
#include "core/common.h"
#include "math/rectangle.h"
#include "graphics/image.h"

class Simulation;

/**
 * Base entity in game
 */
class Entity {
private:
    /**
     * Unique ID for entity
     */
    long id;

    /**
     * Simulation which this entity belongs
     */
    Simulation* simulation;

    /**
     * Images used by this entity
     */
    std::vector<std::shared_ptr<Image>> images;

public:
    /**
     * Entity position and size
     */
    Rectangle rectangle;

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
};

#endif //OPENE2140_ENTITY_H
