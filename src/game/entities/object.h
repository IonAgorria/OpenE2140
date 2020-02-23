//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_OBJECT_H
#define OPENE2140_OBJECT_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/entity.h"
#include "game/components/palette_component.h"
#include "game/components/sprite_damage_component.h"
#include "game/components/movement_component.h"

/**
 * Simple object with simple image
 */
CLASS_ENTITY_COMPONENTS(Entity, Object,
                        ImageComponent,
                        PaletteComponent)
public:
    void simulationChanged() override;

    void draw() override;
};

/**
 * Tree object
 */
CLASS_ENTITY_COMPONENTS(Entity, Tree,
                        ImageComponent,
                        SpriteDamageComponent,
                        PaletteComponent)
public:
    void simulationChanged() override;

    void draw() override;
};

/**
 * Ore object
 */
CLASS_ENTITY_COMPONENTS(Entity, Ore,
                        ImageComponent,
                        MovementComponent,
                        PaletteComponent)
public:
    void draw() override;
};

/**
 * Wall object
 */
CLASS_ENTITY_COMPONENTS(Entity, Wall,
                        ImageComponentSlotted<0>,
                        ImageComponentSlotted<1>,
                        ImageComponentSlotted<2>,
                        ImageComponentSlotted<3>)
public:
    void draw() override;
};

#endif //OPENE2140_OBJECT_H
