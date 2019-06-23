//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/player_component.h"
#include "game/components/player_palette_component.h"
#include "engine/simulation/entities/entity.h"

/**
 * Building entity
 */
CLASS_ENTITY(Entity, Building,
        PlayerComponent,
        PlayerPaletteComponent<Building>
)
public:
    Building();

    void draw() override;
};

#endif //OPENE2140_BUILDING_H
