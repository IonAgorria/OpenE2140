//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/components/faction_component.h"
#include "engine/simulation/components/player_component.h"
#include "engine/simulation/components/attachment_component.h"
#include "game/components/palette_component.h"
#include "src/engine/simulation/entity.h"

/**
 * Building entity
 */
CLASS_ENTITY_COMPONENTS(Entity, Building,
                        PlayerComponent,
                        FactionComponent,
                        ImageComponent,
                        PaletteComponent,
                        AttachmentComponent
)
public:
    void simulationChanged() override;

    void draw() override;

    /**
     * Selects current sprite
     */
    void chooseSprite();
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
};

/**
 * Refinery object, processes ore from mine to money
 */
CLASS_ENTITY(Building, Refinery)
public:
};

#endif //OPENE2140_BUILDING_H
