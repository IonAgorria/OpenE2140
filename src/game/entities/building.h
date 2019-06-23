//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/player_component.h"
#include "engine/simulation/components/image_component.h"
#include "game/components/building_component.h"
#include "engine/simulation/entity.h"

/**
 * Adds player data to entity and handles palette changes
 */
CLASS_ENTITY(Building, PlayerComponent, ImageComponent, BuildingComponent)
public:
    Building();
};

#endif //OPENE2140_BUILDING_H
