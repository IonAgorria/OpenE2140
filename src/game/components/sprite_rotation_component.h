//
// Created by Ion Agorria on 12/09/19
//
#ifndef OPENE2140_SPRITE_ROTATION_COMPONENT_H
#define OPENE2140_SPRITE_ROTATION_COMPONENT_H

#include "engine/core/types.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/entity.h"

class SpriteRotationComponentCommon {
protected:
    virtual ~SpriteRotationComponentCommon() = default;

    /**
     * The sprite rotation index
     */
    uint16_t spriteIndex = 0;

    /**
     * The sprite direction that is the result of rotation index
     */
    entity_direction_t spriteDirection;

    /**
     * @return entity sprite direction
     */
    entity_direction_t getSpriteDirection() const;

    /**
     * Called to check the current sprite rotation
     * @param base
     */
    void updateSpriteIndex(Entity* base);

    /**
     * Selects current sprite
     */
    virtual void chooseSprite() = 0;
};

/**
 * Controls the rotation of entity
 */
class SpriteRotationComponent: public SpriteRotationComponentCommon {
CLASS_COMPONENT(Entity, SpriteRotationComponent)
protected:
    void chooseSprite() override;
};

#endif //OPENE2140_SPRITE_ROTATION_COMPONENT_H
