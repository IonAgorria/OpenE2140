//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/entities/entity_factory.h"
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
    const std::string getConfigPath() override {
        return std::string("data") + DIR_SEP + "buildings.json";
    }

    entity_kind_t getKind() override {
        return ENTITY_KIND_BUILDING;
    }

    std::shared_ptr<Entity> instanceEntity(entity_type_id_t id) override;
};

#endif //OPENE2140_BUILDING_H
