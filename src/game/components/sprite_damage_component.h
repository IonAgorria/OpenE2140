//
// Created by Ion Agorria on 08/09/19
//
#ifndef OPENE2140_SPRITE_DAMAGE_COMPONENT_H
#define OPENE2140_SPRITE_DAMAGE_COMPONENT_H

#include "engine/core/types.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/entity.h"

/**
 * Updates the sprites according to entity health state
 */
CLASS_COMPONENT(Entity, SpriteDamageComponent)
protected:
    /**
     * Selects current sprite
     */
    void chooseSprite();

public:
    /**
     * Threshold which an entity should use the damaged sprite
     */
    entity_health_t damageHealth = 0;
};

#endif //OPENE2140_SPRITE_DAMAGE_COMPONENT_H
