//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_OBJECT_H
#define OPENE2140_OBJECT_H

#include "game/core/constants.h"
#include "engine/simulation/entities/entity_factory.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/entities/entity.h"
#include "game/components/palette_component.h"

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
CLASS_ENTITY(Object, Tree)
public:
};

#endif //OPENE2140_OBJECT_H
