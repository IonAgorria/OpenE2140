//
// Created by Ion Agorria on 17/08/19
//
#ifndef OPENE2140_UNIT_H
#define OPENE2140_UNIT_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/faction_component.h"
#include "engine/simulation/components/player_component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/components/attachment_component.h"
#include "game/components/palette_component.h"
#include "game/components/movement_component.h"
#include "src/engine/simulation/entity.h"

/**
 * Simple object with simple image
 */
CLASS_ENTITY_COMPONENTS(Entity, Unit,
                        PlayerComponent,
                        FactionComponent,
                        ImageComponent,
                        PaletteComponent,
                        AttachmentComponent,
                        MovementComponent)
public:
    void simulationChanged() override;

    void draw() override;
};

#endif //OPENE2140_UNIT_H
