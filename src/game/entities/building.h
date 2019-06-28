//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "src/engine/simulation/entities/entity_factory.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/components/player_component.h"
#include "game/components/player_palette_component.h"
#include "game/core/constants.h"
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

/**
 * Building factory
 */
class BuildingFactory: public IEntityFactory {
    entity_kind_t getKind() {
        return ENTITY_KIND_BUILDING;
    }

    std::shared_ptr<Entity> makeEntity(entity_type_id_t id);
};

#endif //OPENE2140_BUILDING_H
