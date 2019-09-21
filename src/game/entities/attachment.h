//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_ATTACHMENT_H
#define OPENE2140_ATTACHMENT_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/entity.h"
#include "game/components/palette_component.h"
#include "game/components/sprite_rotation_component.h"

/**
 * Spinner object that is constantly animated to gimmick a rotating object
 */
CLASS_ENTITY_COMPONENTS(Entity, Spinner, ImageComponent)
    bool applyCorrection = false;

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
    void simulationChanged() override;

    void update() override;

    void draw() override;

    /**
     * Controls if animation is left or right
     */
    void setDirection(bool left);

    /**
     * Controls if animation is left or right
     */
    void setRunning(bool state);
};

/**
 * Building exit opened when entity comes out
 *
 * TODO copy health values from water base so both show as damaged
 */
CLASS_ENTITY_COMPONENTS(Entity, BuildingExit,
                        ImageComponent)
public:
    void update() override;

    void draw() override;
};

/**
 * Building exit where entities come from underground
 *
 */
//TODO there should be a way to mask a image, maybe redrawing world tiles under? changing renderer?
CLASS_ENTITY_COMPONENTS(Entity, BuildingExitUnderground,
                        ImageComponentSlotted<0>,
                        ImageComponentSlotted<1>,
                        ImageComponentSlotted<2>)
public:
    void update() override;

    void draw() override;
};

/**
 * Turret attachment for an armed entity
 */
CLASS_ENTITY_COMPONENTS(Entity, Turret,
                        ImageComponent,
                        SpriteRotationComponent)
    void simulationChanged() override;

    void update() override;

    void draw() override;
};

#endif //OPENE2140_ATTACHMENT_H
