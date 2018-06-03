//
// Created by Ion Agorria on 2018/06/03
//
#ifndef OPENE2140_ENTITY_H
#define OPENE2140_ENTITY_H

#include "core/config.h"
#include "core/math/rectangle.h"
#include "assets/manager.h"
#include "core/graphics/image.h"

/**
 * Base entity in game
 */
class Entity {
private:
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
     * Loads assets from provided manager
     */
    virtual bool loadAssets(const Manager& manager);

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
