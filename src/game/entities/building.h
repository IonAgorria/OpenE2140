//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/components/faction_component.h"
#include "engine/simulation/components/player_component.h"
#include "engine/simulation/components/power_component.h"
#include "engine/simulation/components/attachment_component.h"
#include "game/components/palette_component.h"
#include "game/components/sprite_damage_component.h"
#include "src/engine/simulation/entity.h"

/**
 * Building entity
 */
CLASS_ENTITY_COMPONENTS(Entity, Building,
                        PlayerComponent,
                        FactionComponent,
                        ImageComponent,
                        PaletteComponent,
                        PowerComponent,
                        SpriteDamageComponent,
                        AttachmentComponent
)
public:
    void simulationChanged() override;

    void draw() override;
};

/**
 * Factory object, manufactures mobile units
 */
CLASS_ENTITY(Building, Factory)
public:
};

/**
 * Mine object, produces ore from ground deposits
 */
CLASS_ENTITY(Building, Mine)
public:
    void simulationChanged() override;
};

/**
 * Refinery object, processes ore from mine to money
 */
CLASS_ENTITY(Building, Refinery)
public:
    void simulationChanged() override;
};

#endif //OPENE2140_BUILDING_H
