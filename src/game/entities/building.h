//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/components/faction_component.h"
#include "engine/simulation/components/player_component.h"
#include "game/components/palette_component.h"
#include "engine/simulation/entities/entity.h"

class Building;
CLASS_COMPONENT(Building, BuildingComponent)
};

/**
 * Building entity
 */
CLASS_ENTITY_COMPONENTS(Entity, Building,
                        PlayerComponent,
                        FactionComponent,
                        ImageComponent,
                        PaletteComponent
)
public:
    void simulationChanged() override;

    void draw() override;

    /**
     * Selects current sprite
     */
    void chooseSprite();
};

#endif //OPENE2140_BUILDING_H
