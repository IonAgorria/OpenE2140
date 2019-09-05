//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_ATTACHMENT_H
#define OPENE2140_ATTACHMENT_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "src/engine/simulation/entity.h"
#include "game/components/palette_component.h"

/**
 * Spinner object that is constantly animated to gimmick a rotating object
 */
CLASS_ENTITY_COMPONENTS(Entity, Spinner, ImageComponent)
    /**
     * Controls if spinner is clockwise or counterclockwise
     */
    bool clockwise = true;

    void simulationChanged() override;

    void update() override;

    void draw() override;
};

/**
 * Conveyor belt object used in mine and refinery
 */
CLASS_ENTITY_COMPONENTS(Entity, ConveyorBelt,
                      ImageComponentSlotted<0>,
                      ImageComponentSlotted<1>)
public:
    /**
     * Controls if animation is left or right
     * TODO set this to default anim direction
     */
    bool left = false;

    void simulationChanged() override;

    void update() override;

    void draw() override;
};

/**
 * Turret attachment for an armed entity
 */
CLASS_ENTITY_COMPONENTS(Entity, Turret, ImageComponent)
    void simulationChanged() override;

    void update() override;

    void draw() override;
};

#endif //OPENE2140_ATTACHMENT_H
