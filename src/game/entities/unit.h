//
// Created by Ion Agorria on 17/08/19
//
#ifndef OPENE2140_UNIT_H
#define OPENE2140_UNIT_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/entities/entity.h"
#include "game/components/palette_component.h"

/**
 * Simple object with simple image
 */
CLASS_ENTITY_COMPONENTS(Entity, Unit,
                        ImageComponent,
                        PaletteComponent)
public:
    void simulationChanged() override;

    void draw() override;
};

#endif //OPENE2140_UNIT_H
